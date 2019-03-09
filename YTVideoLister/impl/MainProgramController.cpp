
#include "MainProgramController.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <nlohmann/json.hpp>

#include "ConsoleUtils.hpp"
#include "HttpsRequestSender.hpp"
#include "ProgramConfig.hpp"

namespace jcul {

    MainProgramController::MainProgramController(const ProgramConfig & config, const std::string & apiKey, const std::string & pathToOutputFile)
        : m_programConfig(config)
        , m_apiKey(apiKey)
        , m_outputFilePath(pathToOutputFile)
        , m_nextPageToken("")
        , m_httpsReqSender()
        , m_videoInfos()
        , m_totalResults(-1)
        , m_currentPage(0u)
        , m_outputFile()
    {
    }

    MainProgramController::~MainProgramController() {
    
        if (m_outputFile.is_open()) {
            m_outputFile.close();
        }
    }

    void MainProgramController::runProgram() {
        
        // Reset program state:
        m_nextPageToken.clear();
        m_videoInfos.clear();
        m_totalResults = -1;
        m_currentPage  = 0u;

        // Open output file
        m_outputFile.open(m_outputFilePath, std::ofstream::out);
        if (!m_outputFile.is_open()) {
            throw std::runtime_error("Could not open output file!");
        }

        // Fetch 1st page:
        fetchNextPage();

        // Main program loop:
        while (true) {
        
            updateVideoInfosForPage(m_currentPage);

            ConsoleUtils::ClearConsole();

            std::cout << "Showing videos ";
            std::cout << (m_programConfig.resultsPerPage * m_currentPage + 1);
            std::cout << " through ";
            std::cout << (m_programConfig.resultsPerPage * (m_currentPage + 1u));
            std::cout << " (out of " << m_totalResults << " total).\n\n";

            for (unsigned i = 0u; i < m_programConfig.resultsPerPage; i += 1) {
            
                unsigned index = (m_programConfig.resultsPerPage * m_currentPage) + i;

                if (m_videoInfos.size() <= index) break;

                std::cout << (index + 1) << ".\n";
                std::cout << m_videoInfos[index].name << "\n";

            }

            std::cout << "\nPress N for next page, P for previous page, or Q to quit.\n";

            while (true) {
            
                int c = ConsoleUtils::GetCharNoEnter();

                if (c == 'p' || c == 'P') {
                    if (m_currentPage > 0u) m_currentPage -= 1u;
                    break;
                }
                else if (c == 'n' || c == 'N') {
                    m_currentPage += 1u;
                    break;
                }
                else if (c == 'q' || c == 'Q') {
                    return; // End of program
                }
                else {
                    continue;
                }

            }

        }

        // Close output file
        m_outputFile.close();

    }

    std::string MainProgramController::makeHttpsRequest() {
    
        std::string request = "https://www.googleapis.com/youtube/v3/search"; // base URL
        request += "?part=snippet%2Cid"; // part = snippet, id
        request += "&channelId=" + m_programConfig.channelId;
        request += "&maxResults=" + std::to_string(m_programConfig.resultsPerPage);
        request += "&order=date";        // order newest-to-oldest
        request += "&type=video";        // we want only videos
        request += "&key=" + m_apiKey;   // authorize request with API key

        if (!m_nextPageToken.empty()) {
            request += "&pageToken=" + m_nextPageToken;
        }

        return request;

    }

    void MainProgramController::fetchNextPage() {
    
        using JSON = nlohmann::json;

        std::string request  = makeHttpsRequest();
        std::cout << "Sending HTTPS request...\n";
        std::string response = m_httpsReqSender.sendGetRequest(request);
        std::cout << "Done!\n";

        JSON doc = JSON::parse(response);

        m_nextPageToken = doc["nextPageToken"].get<std::string>();
        m_totalResults  = doc["pageInfo"]["totalResults"].get<int>();

        for (auto & item : doc["items"]) {

            std::string name = item["snippet"]["title"];
            std::string id   = item["id"]["videoId"];
            std::string desc = item["snippet"]["description"];

            gotNewVideoInfo(name, id, desc);

        }

    }

    void MainProgramController::updateVideoInfosForPage(unsigned pageNumber) {
    
        while (m_videoInfos.size() < (pageNumber + 1u) * m_programConfig.resultsPerPage) {

            fetchNextPage();

            if (m_totalResults != -1 && m_videoInfos.size() >= (unsigned)m_totalResults) break;

        }

    }

    void MainProgramController::gotNewVideoInfo(const std::string & name, const std::string & id, const std::string & desc) {
    
        m_videoInfos.emplace_back(name, id, desc);

        m_outputFile << m_videoInfos.size() << ".\n";
        m_outputFile << "NAME: " << name << "\n";
        m_outputFile << "LINK: " << "https://www.youtube.com/watch?v=" << id << "\n";
        m_outputFile << "DESC: " << desc << "\n\n";

    }

}