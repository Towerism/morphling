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

GCPSocket::~GCPSocket() {
    disconnect();
}

// ======================================================================
// Private Functions
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

std::string GCPSocket::sread() {
    char buffer[MAX_MESSAGE];

    memset(buffer, 0, MAX_MESSAGE);

    struct timeval r_timeout;
    r_timeout.tv_sec = 2;
    r_timeout.tv_usec = 0;
    
    fd_set read_fd;
    FD_ZERO(&read_fd);
    FD_SET(_sockfd, &read_fd);

    int rv = select(_sockfd+1, &read_fd, nullptr, nullptr, &r_timeout);
    if (rv > 0) {
        int res = recv(_sockfd, buffer, MAX_MESSAGE - 2, 0);
        if (res > 0) {
            std::string read_s = std::string(buffer);
            return read_s;
        } else {
            printf("Bad sread(): disconnecting");
            disconnect();
        }
    } else if (rv == 0) {
        // timeout occured
    } else {
        // error occured
        std::cout << "Select returned: " << std::to_string(rv) << std::endl;
        disconnect();
        return "";
    }

    return "";
}

bool GCPSocket::swrite(std::string msg) {
    if (_connected) {
        if (send(_sockfd, msg.c_str(), strlen(msg.c_str()), 0) == -1) {
            return false;
        }
    }

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
        perror("Could not connect to hostname");
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

void GCPSocket::send_auth(std::string auth) {
    if (_connected) {
        swrite("AUTH:"+auth+"\n");
    }
}


