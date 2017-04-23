#pragma once

#include <curl/curl.h>
#include <thirdparty/json.hpp>

#include <iostream>
#include <string>

namespace Morphling::Database {

using json = nlohmann::json;

// Error return struct to provide CURL return code and the response 
// from firebase as a json object
struct fire_err {
    CURLcode res_code;
    json res_json;
};

class firebase {
private:
    // Curl header list
    struct curl_slist* curl_chunk;

    // Curl objects for PUT, POST, GET, PATCH
    CURL* curlPUT;
    CURL* curlPATCH;
    CURL* curlGET;

    // initialization functions for CURL HTTP calls
    void init_put();
    void init_patch();
    void init_get();

    // project id in firebase
    std::string proj_id;
    // full url of the firebase database
    std::string base_url;

    struct write_object {
        const char *readptr;
        int sizeleft;
    };
    

    // CURL read function for reading a response into a std::string
    static size_t curl_readfunc
    (
        void *ptr,
        size_t size,
        size_t nmemb,
        void *userp
    );

    // CURL write function to write a write_object to the payload 
    // (PUT, POST)
    static size_t curl_writefunc
    (
        void *contents, 
        size_t size, 
        size_t nmemb, 
        std::string *s
    );
public:
    // Initialize the firebase object with the CURL objects to target the 
    // https://project_id.firebaseio.com/<path>.json
    firebase
    (
        std::string project_id
    );
    
    // Deconstruct the CURL objects properly and the CURL global cleanup
    ~firebase();


    // Write a key-value pair translated into json before transmition.
    //
    // This will override any current object in-place for the current 
    // <path> presented.
    //
    // How path is appended to the base_url:
    //     https://project_id.firebaseio.com/<path>.json
    fire_err write_kv
    (
        std::string path, 
        std::string key, 
        std::string value
    );
    
    // Write a json object to the associated path in the firebase 
    // database.
    //
    // This will override any current object in-place for the current 
    // <path> presented.
    //
    // How path is appended to the base_url:
    //     https://project_id.firebaseio.com/<path>.json
    fire_err write_json
    (
        std::string path, 
        json data_json
    );

    // Update a json object to the associated path in the firebase 
    // database.
    //
    // This will override any current object in-place for the current 
    // <path> presented, but not delete any objects that are omitted.
    //
    // How path is appended to the base_url:
    //     https://project_id.firebaseio.com/<path>.json
    fire_err update_json
    (
        std::string path, 
        json data_json
    );

    // Get a json object at <path> from firebase database.
    //
    // The param 'shallow' determines whether to return the full object
    // tree or just the root request.
    // Example /test.json?shallow=true for {"test": { "a": 0 } }
    // will return {"test": true}
    // but /test.json will normally return the nested object: {"a": 0}
    //
    // How path is appended to the base_url:
    //     https://project_id.firebaseio.com/<path>.json
    fire_err get_json
    (
        std::string path,
        bool shallow = false
    );
}; // end class firebase

} // end namespace Morphling::database
