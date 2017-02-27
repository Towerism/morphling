#include <curl/curl.h>
#include <thirdparty/json.hpp>

#include <iostream>
#include <string>

using json = nlohmann::json;

struct write_object {
    const char *readptr;
    int sizeleft;
};

struct fire_err {
    CURLcode res_code;
    json res_json;
};


class firebase {
private:
    struct curl_slist* curl_chunk;
    CURL* curlPUT;

    std::string proj_id;
    std::string base_url;

    static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *userp);
    static size_t curl_writefunc(void *contents, size_t size, size_t nmemb, std::string *s);

public:
    firebase(std::string project_id);
    ~firebase();

    // initialization functions for CURL HTTP calls
    void init_put();

    fire_err write_kv(std::string path, std::string key, std::string value);
    fire_err write_json(std::string path, json data_json);

}; // end class firebase
