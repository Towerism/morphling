#include "firebase.h"

firebase::firebase(std::string project_id): 
    proj_id(project_id),
    curl_chunk(NULL),
    base_url("https://"+project_id+".firebaseio.com/")
{
    // set chunked data
    curl_chunk = curl_slist_append(curl_chunk, "Transfer-Encoding: chunked");
    // init the curl objects
    init_put();
}

firebase::~firebase() {
    curl_easy_cleanup(curlPUT);
    // free the custom headers
    curl_slist_free_all(curl_chunk);
    // global curl cleanup
    curl_global_cleanup();
}

void firebase::init_put() {
    CURLcode res;
    // initialize curl object
    curlPUT = curl_easy_init();
    if (curlPUT) {
        // set return options
        res = curl_easy_setopt(curlPUT, CURLOPT_WRITEFUNCTION, curl_writefunc);
        //res = curl_easy_setopt(curlPUT, CURLOPT_VERBOSE, 1L);
        // Enable uploading
        res = curl_easy_setopt(curlPUT, CURLOPT_UPLOAD, 1L);
        // HTTP PUT
        res = curl_easy_setopt(curlPUT, CURLOPT_PUT, 1L);
        // set read function
        res = curl_easy_setopt(curlPUT, CURLOPT_READFUNCTION, read_callback);
        // set http header for chunked responses
        res = curl_easy_setopt(curlPUT, CURLOPT_HTTPHEADER, curl_chunk);
    }
}

fire_err firebase::write_kv(std::string path, std::string key, std::string value) {
    fire_err fe;
    fe.res_code = CURLE_OK;
    fe.res_json = json("{}");

    return fe;
}

fire_err firebase::write_json(std::string path, json data_json) {
    struct write_object wo;

    std::string data = data_json.dump();
    wo.readptr = data.c_str();
    wo.sizeleft = data.size();

    // specify which file to upload
    curl_easy_setopt(curlPUT, CURLOPT_READDATA, &wo);

    std::string response;
    curl_easy_setopt(curlPUT, CURLOPT_WRITEDATA, &response);

    // set target url
    std::string target_url = base_url + path;
    curl_easy_setopt(curlPUT, CURLOPT_URL, target_url.c_str());

    // preform http call
    fire_err fe;
    fe.res_code = curl_easy_perform(curlPUT);
    
    // check for errors
    if (fe.res_code != CURLE_OK) {
        fe.res_json = json::parse("{}");
    } else {
        fe.res_json = json::parse(response);
    }

    return fe;
}

size_t firebase::read_callback(void *ptr, size_t size, size_t nmemb, void *userp)
{
    struct write_object* wt = (struct write_object*)userp;

    if(size*nmemb < 1)
        return 0;

    if(wt->sizeleft) {
        *(char *)ptr = wt->readptr[0]; // copy one single byte
        wt->readptr++;                 // advance pointer
        wt->sizeleft--;                // less data left
        return 1;                      // we return 1 byte at a time!
    }

    return 0;                          // no more data left to deliver
}

// Curl write function borrowed from http://stackoverflow.com/questions/2329571/c-libcurl-get-output-into-a-string
size_t firebase::curl_writefunc(void *contents, size_t size, size_t nmemb, std::string *s)
{
    size_t newLength = size*nmemb;
    size_t oldLength = s->size();
    try
    {
        s->resize(oldLength + newLength);
    }
    catch(std::bad_alloc &e)
    {
        // handle memory problem
        return 0;
    }

    std::copy((char*)contents,(char*)contents+newLength,s->begin()+oldLength);
    return size*nmemb;
}

