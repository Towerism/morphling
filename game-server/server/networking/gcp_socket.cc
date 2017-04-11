#include <networking/gcp_socket.h>

using namespace Morphling::Networking;

GCPSocket::GCPSocket(): _connected(false) {

    if ((_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        printf("Socket create failure.");
        exit(-1);
    }

    int on = 1;
    if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, (char const*)&on, sizeof(on)) == -1) {
        printf("Invalid socket.");
        exit(-1);
    }

    fcntl(_sockfd, F_SETFL, O_NONBLOCK);
    fcntl(_sockfd, F_SETFL, O_ASYNC);
}

GCPSocket::GCPSocket(int sockfd): _sockfd(sockfd), _connected(true) {}

GCPSocket::~GCPSocket() { }

// ======================================================================
// Protected Functions
// ======================================================================

bool GCPSocket::dns(std::string hostname, int port, struct sockaddr_in* server) {
    // structure used in DNS lookups
    struct hostent *remote;

    // first assume that the string is an IP address
    int rv = inet_pton(AF_INET,hostname.c_str(),&(server->sin_addr));
    if (rv != 1) {
        // if not a valid IP, then do a DNS lookup
        if ((remote = gethostbyname(hostname.c_str())) == NULL) {
            //printf("Invalid string: neither FQDN, nor IP address (%d)\n",WSAGetLastError());
            return false;
        } else {
            // take the first IP address and copy into sin_addr
            memcpy((char *)&(server->sin_addr), remote->h_addr, remote->h_length);
        }
    }

    // setup the port # and protocol type
    server->sin_family = AF_INET;
    server->sin_port = htons(port);

    // mark finish of DNS
    return true;
}

// ======================================================================
// Public Functions
// ======================================================================

bool GCPSocket::connect(std::string hostname, int port) {
    // if already connected, don't connect again
    if (_connected) { return false; }

    sockaddr_in addr;

    if (!dns(hostname,port,&addr)) {
        printf("Could not resolve hostname");
        return false;
    }

    if (::connect(_sockfd, (sockaddr*)&addr, sizeof(addr)) == -1) {
        printf("Could not connect to hostname");
        close(_sockfd);
        return false;
    }

    _connected = true;
    return _connected;
}

void GCPSocket::disconnect() {
    if (_connected) {
        close(_sockfd);
        _connected = false;
    }
}

GCPSocket::RET GCPSocket::sread()
{
    char buffer[MAX_MESSAGE];

    memset(buffer, 0, MAX_MESSAGE);

    struct timeval r_timeout;
    r_timeout.tv_sec = MAX_TIMEOUT;
    r_timeout.tv_usec = 0;
    
    fd_set read_fd;
    FD_ZERO(&read_fd);
    FD_SET(_sockfd, &read_fd);

    int rv = select(_sockfd+1, &read_fd, nullptr, nullptr, &r_timeout);
    if (rv > 0) {
        int res = recv(_sockfd, buffer, MAX_MESSAGE, MSG_WAITALL);
        if (res > 0) {
            std::string read_s = std::string(buffer);
            // normal read
            return std::make_tuple(Ok,read_s);
        } else {
            // bad read
            return std::make_tuple(Error,"Error: Bad socket read");
        }
    } else if (rv == 0) {
        // timeout occured
        return std::make_tuple(Timeout,"");
    } else {
        // error occured
        return std::make_tuple(Error,"Error: Socket select returned "+std::to_string(rv));
    }
}

GCPSocket::RET GCPSocket::sread_wait(time_t seconds, size_t tries)
{
    char buffer[MAX_MESSAGE];
    memset(buffer, 0, MAX_MESSAGE);

    // make sure to set an upper bound for the MAX_TIMEOUT
    if (seconds > MAX_TIMEOUT) {
        seconds = MAX_TIMEOUT;
    }
    // make sure upper bound set for max_tries
    if (tries > MAX_TRIES) {
        tries = MAX_TRIES;
    }
    size_t total_tries = tries;

    struct timeval r_timeout;
    r_timeout.tv_sec = seconds;
    r_timeout.tv_usec = 0;

    fd_set read_fd;

    while (tries != 0 and _connected) {
    
        FD_ZERO(&read_fd);
        FD_SET(_sockfd, &read_fd);

        int rv = select(_sockfd+1, &read_fd, nullptr, nullptr, &r_timeout);
        if (rv > 0) {
            int res = recv(_sockfd, buffer, MAX_MESSAGE, MSG_WAITALL);
            if (res > 0) {
                std::string read_s = std::string(buffer);
                //std::cerr << "Msg: " << read_s << std::endl;
                // normal read
                return std::make_tuple(Ok,read_s);
            } else {
                // bad read
                return std::make_tuple(Error,"Error: Bad socket read");
            }
        } else if (rv == 0) {
            // timeout occured but we still want to wait for a response
            tries--;
        } else {
            // error occured
            return std::make_tuple(Error,"Error: Socket select returned "+std::to_string(rv));
        }
    }
    return std::make_tuple(Timeout,"Timeout exceeded: duration = "+std::to_string(seconds*total_tries));
}

GCPSocket::RET GCPSocket::swrite(std::string msg)
{
    if (_connected) {
        char buf[MAX_MESSAGE];
        memset(buf,'\0',MAX_MESSAGE);
        memcpy(buf,msg.c_str(),msg.size());
        int total = 0;                  // how many bytes we've sent
        int bytesleft = MAX_MESSAGE;  // how many we have left to send
        int n;

        while(total < MAX_MESSAGE) {
            n = send(_sockfd, buf+total, bytesleft, 0);
            if (n == -1) { break; }
            total += n;
            bytesleft -= n;
        }

        if (n == -1) {
            return std::make_tuple(Error,std::string(strerror(errno)));
        }
    } else {
        return std::make_tuple(Error,"Not Connected, cannot write socket.");
    }

    return std::make_tuple(Ok,"");
}

// Read a line from the GCP socket and parse the sent Tag.
// Example socket read: "AUTH:asdf"
// will return "asdf" for input of Tag: "AUTH"
GCPSocket::RET GCPSocket::read_tag(std::string tag) {
    auto response = sread_wait();
    // check if the response is Ok
    if (std::get<0>(response) != Ok)
        return std::make_tuple(Error,std::get<1>(response));
    
    std::string msg = std::get<1>(response);
    size_t colon = msg.find(":");
    if (colon == std::string::npos)
        return std::make_tuple(Error,"Error: no tag sent");

    // parse the received tag
    std::string recv_tag = msg.substr(0,colon);
    std::string value = msg.substr(colon+1);
    // verify the tag
    if (recv_tag == tag) {
        // return the value
        return std::make_tuple(Ok,value);
    } else {
        // wrong tag received
        return std::make_tuple(Error,"Error: invalid tag("+tag+"), recv: "+recv_tag);
    }
}

// Read a line from the GCP socket and parse the sent Tag.
// Example socket read: "AUTH:asdf"
// will return "asdf" for input of Tag: "AUTH"
GCPSocket::RTAGS GCPSocket::read_tags(std::string tag1, std::string tag2) {
    auto response = sread_wait(20);
    // check if the response is Ok
    if (std::get<0>(response) != Ok)
        return std::make_tuple(Error,std::get<1>(response),"");
    
    std::string msg = std::get<1>(response);
    // find the first colon
    size_t colon1 = msg.find(":");
    if (colon1 == std::string::npos)
        return std::make_tuple(Error,"Error: no tags","");
    // find the ~ divider
    size_t sq = msg.find("~",colon1+1);
    if (sq == std::string::npos)
        return std::make_tuple(Error,"Error: not two msgs sent","");
    // find the second colon
    size_t colon2 = msg.find(":",sq+1);
    if (colon2 == std::string::npos)
        return std::make_tuple(Error,"Error: no second tag","");

    size_t bn = msg.find("\n",colon2+1); 

    // parse the received tag
    std::string rtag1 = msg.substr(0,colon1);
    std::string val1 = msg.substr(colon1+1,sq-colon1-1);
    std::string rtag2 = msg.substr(sq+1,colon2-sq-1);
    std::string val2;
    if (bn == std::string::npos) {
        // if they didn't end with \n
        val2 = msg.substr(colon2+1);
    } else {
        // if they did end with \n
        val2 = msg.substr(colon2+1,bn-colon2-1);
    }
    // verify the tag
    if (rtag1 == tag1 and rtag2 == tag2) {
        // return the value
        return std::make_tuple(Ok,val1,val2);
    } else {
        // wrong tag received
        return std::make_tuple(Error,"Error: invalid tag1("+tag1+"=/="+rtag1+") or tag2("+tag2+"=/="+rtag2+")","");
    }
}