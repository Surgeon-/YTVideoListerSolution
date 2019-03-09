#pragma once

#include <deque>
#include <fstream>
#include <string>

#include "HttpsRequestSender.hpp"
#include "ProgramConfig.hpp"

namespace jcul {

    class MainProgramController {
    
    public:

        MainProgramController(const ProgramConfig & config, const std::string & apiKey, const std::string & pathToOutputFile);

        ~MainProgramController();

        void runProgram();

    private:

        struct VideoInfo {
        
            std::string name;
            std::string id;
            std::string desc;

            VideoInfo(const std::string & name, const std::string & id, const std::string & desc)
                : name(name)
                , id(id)
                , desc(desc)
            {
            }

        };

        ProgramConfig m_programConfig;
        std::string   m_apiKey;
        std::string   m_outputFilePath;
        std::string   m_nextPageToken;
        HttpsRequestSender m_httpsReqSender;
        std::deque<VideoInfo> m_videoInfos;
        int           m_totalResults;
        unsigned      m_currentPage;
        std::ofstream m_outputFile;

        std::string makeHttpsRequest();
        void fetchNextPage();
        void updateVideoInfosForPage(unsigned pageNumber);
        void gotNewVideoInfo(const std::string & name, const std::string & id, const std::string & desc);
    
    };


}