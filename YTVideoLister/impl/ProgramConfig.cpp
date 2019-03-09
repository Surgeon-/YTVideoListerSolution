
#include "ProgramConfig.hpp"

#include <iostream>
#include <fstream>
#include <regex>
#include <stdexcept>

namespace jcul {

    namespace {
    
        void KeyValuePairToProgramConfig(ProgramConfig & config, const std::string & key, const std::string & value) {
        
            if (key == CONFIG_KEY_CHANNEL_ID) {
                config.channelId = value;
                return;
            }
            
            if (key == CONFIG_KEY_RESULTS_PER_PAGE) {
                unsigned number = static_cast<unsigned>(std::stoi(value));
                if (number < 1 || number > 50) {
                    throw std::runtime_error("KeyValuePairToProgramConfig - RESULTS_PER_PAGE must be between 1 and 50 (inclusive)!");
                }
                config.resultsPerPage = number;
                return;
            }

            std::cout << "Unknown key found in config file (" << key << ") - it will be ignored.\n";

        }

    }

    ProgramConfig ParseConfigFile(const std::string & path) {
    
        std::ifstream file{path.c_str(), std::ifstream::in};
        if (!file.is_open()) {
            throw std::runtime_error("Could not open config file!");
        }

        ProgramConfig rv;

        std::string line;
        while (std::getline(file, line)) {
        
            static const char * PATTERN_EMPTY = R"_(^\s*(//.*)?)_";
            static const char * PATTERN_VALUE = R"_(^\s*([a-zA-Z0-9_]+)\s*=\s*"(.+)"\s*(//.*)?)_";

            std::smatch matches;
            if (std::regex_match(line, matches, std::regex(PATTERN_EMPTY))) {
                continue;
            }
            else if (std::regex_match(line, matches, std::regex(PATTERN_VALUE))) {
                // matches[1] = key
                // matches[2] = value
                KeyValuePairToProgramConfig(rv, matches[1], matches[2]);
            }
            else {
                throw std::runtime_error("ParseConfigFile - Unknown format in line [" + line + "]!");
            }

        }

        return rv;

    }

}