#pragma once

#include <string>

namespace jcul {

    class ConsoleUtils {
    
    public:

        static void WaitAnyKey(const std::string & prompt);

        static int GetCharNoEnter();

        static void ClearConsole();
    
    };

}