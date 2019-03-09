
#include <iostream>
#include <cstring>
#include <memory>
#include <stdexcept>

#include "ConsoleUtils.hpp"
#include "GlobalCurlGuard.hpp"
#include "MainProgramController.hpp"
#include "ProgramConfig.hpp"

using namespace jcul;
using std::cin;
using std::cout;

template<typename ... Args>
std::string StringFromFormat( const std::string& format, Args ... args ) {

    int requiredBufferSize = snprintf(nullptr, 0u, format.c_str(), args ... ) + 1; // Extra space for '\0'

    std::unique_ptr<char[]> buf( new char[ requiredBufferSize ] );

    snprintf( buf.get(), requiredBufferSize, format.c_str(), args ... );

    return std::string( buf.get(), buf.get() + (requiredBufferSize - 1) ); // We don't want the '\0' inside

}

int main() {

    static const std::string MY_API_KEY = "AIzaSyCfc9yLs6zDyZyg7Sc3nbdqqtQE9hsePS0";

    try {

        ProgramConfig programConfig = ParseConfigFile("config.txt");

        GlobalCurlGuard globalCurlGuard{};

        MainProgramController ctrl{programConfig, MY_API_KEY, "output.txt"};

        ctrl.runProgram();

        //ConsoleUtils::WaitAnyKey("Press any key to continue...");

        return EXIT_SUCCESS;

    }
    catch (std::exception& ex) {
    
        cout << "Exception caught: " << ex.what() << "\n";

        ConsoleUtils::WaitAnyKey("Press any key to continue...");

        return EXIT_FAILURE;

    }

}





