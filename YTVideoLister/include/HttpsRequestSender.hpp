#pragma once

#include <string>

namespace jcul {

    using std::size_t;

    class HttpsRequestSender {
    
    public:

        HttpsRequestSender();
        ~HttpsRequestSender();

        std::string sendGetRequest(const std::string & request);

        // Make not movable and not copyable:
        HttpsRequestSender(const HttpsRequestSender & other) = delete;
        HttpsRequestSender& operator=(const HttpsRequestSender & other) = delete;
        HttpsRequestSender(HttpsRequestSender && other) = delete;
        HttpsRequestSender& operator=(HttpsRequestSender && other) = delete;

    private:
    
        void * m_curlInstance;
        std::string m_responseBuffer;

        static size_t writeCallback(char * data, size_t memberSizeof, size_t memberCount, void * userdata);

    };

}