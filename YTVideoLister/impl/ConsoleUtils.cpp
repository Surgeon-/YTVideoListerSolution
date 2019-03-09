
#include "ConsoleUtils.hpp"

#include <iostream>
#include <string>

// Windows-specific
#include <conio.h>
#include <windows.h>

namespace jcul {

    void ConsoleUtils::WaitAnyKey(const std::string & prompt) {
    
        std::cout << prompt;
        _getch();

    }

    int ConsoleUtils::GetCharNoEnter() {
    
        return _getch();

    }

    void ConsoleUtils::ClearConsole() {
    
        system("cls");

    }

}