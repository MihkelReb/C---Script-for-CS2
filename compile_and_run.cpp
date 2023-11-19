#include <Windows.h>
#include <iostream>
#include <string>

int main() {
    // Specify the path to the GCC compiler
    std::string compilerPath = "C:\\msys64\\ucrt64\\bin\\gcc.exe";

    // Specify the command to compile your C++ program with the -lstdc++ flag
    std::string compileCommand = compilerPath + " FlickScript.cpp -o FlickScript.exe -lstdc++";

    // Compile the C++ program
    int compileResult = system(compileCommand.c_str());

    if (compileResult == 0) {
        // Compilation was successful, now you can run your program
        system("FlickScript.exe");

        // Wait for user input to keep the terminal open
        std::cout << "Press Enter to exit...";
        std::cin.get();
    } else {
        std::cerr << "Compilation failed." << std::endl;

        // Wait for user input to keep the terminal open
        std::cout << "Press Enter to exit...";
        std::cin.get();
    }

    return 0;
}
