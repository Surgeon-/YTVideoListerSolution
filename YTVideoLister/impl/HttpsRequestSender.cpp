
#include "HttpsRequestSender.hpp"

#include <string>
#include <curl/curl.h>
#include <stdexcept>

namespace jcul {

    namespace {

        CURL * ToCurlPtr(void * ptr) {
            return static_cast<CURL*>(ptr);
        }    

    }

    HttpsRequestSender::HttpsRequestSender()
        : m_curlInstance(NULL) 
        , m_responseBuffer()
    {

        m_curlInstance = curl_easy_init();
        CURL * curl = ToCurlPtr(m_curlInstance);
        if (curl == NULL) {
            throw std::runtime_error("HttpsRequestSender::HttpsRequestSender - Could not initialize m_curlInstance!");
        }

    }
     
    HttpsRequestSender::~HttpsRequestSender() {
    
        CURL * curl = ToCurlPtr(m_curlInstance);
        if (curl != NULL) {
            curl_easy_cleanup(curl);
        }

    }

    std::string HttpsRequestSender::sendGetRequest(const std::string & request) {
    
        m_responseBuffer.clear();

        CURL * curl = ToCurlPtr(m_curlInstance);

        curl_easy_setopt(curl, CURLOPT_URL, request.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::string msg = "Get request failed: ";
            msg += curl_easy_strerror(res);
            throw std::runtime_error(msg.c_str());
        }

        return m_responseBuffer;

    }

    size_t HttpsRequestSender::writeCallback(char * data, size_t memberSizeof, size_t memberCount, void * userdata) {
    
        HttpsRequestSender * proxyThis = static_cast<HttpsRequestSender*>(userdata);

        size_t realSize = memberCount * memberSizeof;

        proxyThis->m_responseBuffer.append(data, realSize);

        return realSize;

    }

}