
#include "GlobalCurlGuard.hpp"

#include <string>
#include <stdexcept>
#include <curl/curl.h>

namespace jcul {

    GlobalCurlGuard::GlobalCurlGuard() {
        CURLcode status = curl_global_init(CURL_GLOBAL_ALL);
        if (status != CURLE_OK) {
            std::string msg = "GlobalCurlGuard::GlobalCurlGuard - curl_global_init() failed: ";
            msg += curl_easy_strerror(status);
            throw std::runtime_error(msg.c_str());
        }
    }

    GlobalCurlGuard::~GlobalCurlGuard() {
        curl_global_cleanup();
    }

}