#pragma once

#include <string>

namespace jcul {
    
    static const char * CONFIG_KEY_CHANNEL_ID = "CHANNEL_ID";
    static const char * CONFIG_KEY_RESULTS_PER_PAGE = "RESULTS_PER_PAGE";

    struct ProgramConfig {
    
        std::string channelId;
        unsigned    resultsPerPage;

        ProgramConfig()
            : channelId("UCK5eBtuoj_HkdXKHNmBLAXg")
            , resultsPerPage(10)
        {
        }

    };

    ProgramConfig ParseConfigFile(const std::string & path);

}