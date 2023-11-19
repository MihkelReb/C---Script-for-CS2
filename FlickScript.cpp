// Include the Windows-specific header file to access various Windows API functions.
#include <Windows.h>

// Include the standard input-output stream library for C++.
// This allows us to use functions like std::cin, std::cout, etc.
#include <iostream>

// Include the C++ threading library which provides facilities to manage multiple threads.
#include <thread>

// Include the C++ Chrono library which provides time utilities.
// This library can be used to perform time-based actions like waiting for a specific duration.
#include <chrono>


/**
 * Checks if a specific key is currently pressed.
 * 
 * @param key The virtual-key code of the key. For a complete list of codes, see: 
 *            https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
 * @return Returns true if the key is pressed, otherwise returns false.
 */
bool isKeyPressed(int key) {
    // GetAsyncKeyState determines whether a key is up or down at the time 
    // the function is called, and whether the key was pressed after a previous call 
    // to GetAsyncKeyState.
    // The most significant bit (0x8000) will be set if the key is down.
    return GetAsyncKeyState(key) & 0x8000;
}


/**
 * Retrieves the current time in microseconds using the high-resolution performance counter.
 *
 * @return Returns the current time in microseconds.
 */
__int64 GetCurrentTimeMicroseconds() {
    // Declare structures to store the performance counter's frequency and the current tick count.
    LARGE_INTEGER frequency, current_time;

    // Retrieves the frequency of the high-resolution performance counter.
    QueryPerformanceFrequency(&frequency);

    // Retrieves the current value of the high-resolution performance counter.
    QueryPerformanceCounter(&current_time);

    // Convert the current tick count to microseconds by multiplying it by 1,000,000 
    // (to convert seconds to microseconds) and then dividing by the counter's frequency.
    return current_time.QuadPart * 1000000 / frequency.QuadPart;
}


/**
 * Prints the elapsed time an action took to execute.
 *
 * @param actionName   Name or description of the action.
 * @param start_time   Timestamp when the action started, in microseconds.
 * @param end_time     Timestamp when the action ended, in microseconds.
 */
void PrintActionTime(const char* actionName, __int64 start_time, __int64 end_time) {
    
    // Calculate the time difference between the start and end timestamps.
    __int64 elapsed_time = end_time - start_time;

    // If the elapsed time is 10,000 microseconds (10 milliseconds) or greater, display the time in milliseconds.
    if (elapsed_time >= 10000) {  
        std::cout << actionName << " took " << elapsed_time / 10000 << " milliseconds." << std::endl;
    } 
    // Otherwise, display the elapsed time in microseconds.
    else {
        std::cout << actionName << " took " << elapsed_time << " microseconds." << std::endl;
    }
}


/**
 * Pauses the execution for the specified number of microseconds.
 *
 * @param microseconds   The number of microseconds to wait.
 */
void waitMicroseconds(__int64 microseconds) {
    // Record the current time in microseconds.
    __int64 start_time = GetCurrentTimeMicroseconds();
    
    // Continuously check the elapsed time since the start_time.
    while (true) {
        // If the elapsed time has reached or surpassed the specified duration, exit the loop.
        if (GetCurrentTimeMicroseconds() - start_time >= microseconds) {
            break;
        }
    }
}


/**
 * Simulates a mouse right-click using the Windows SendInput function.
 */
void rightClick() {
    // Create an INPUT struct to hold the mouse event data.
    INPUT input;
    
    // Set the type of the INPUT struct to be a mouse event.
    input.type = INPUT_MOUSE;
    
    // Set the mouse event flag to indicate a right-button press down.
    input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
    
    // Send the right-button press down event.
    SendInput(1, &input, sizeof(INPUT));
    
    // Set the mouse event flag to indicate a right-button release.
    input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
    
    // Send the right-button release event.
    SendInput(1, &input, sizeof(INPUT));
}


/**
 * Simulates a mouse left-click using the Windows SendInput function.
 */
void leftClick() {
    // Create an INPUT struct to hold the mouse event data.
    INPUT input;
    
    // Set the type of the INPUT struct to be a mouse event.
    input.type = INPUT_MOUSE;
    
    // Set the mouse event flag to indicate a left-button press down.
    input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    
    // Send the left-button press down event.
    SendInput(1, &input, sizeof(INPUT));
    
    // Set the mouse event flag to indicate a left-button release.
    input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    
    // Send the left-button release event.
    SendInput(1, &input, sizeof(INPUT));
}


/**
 * Moves the mouse cursor horizontally in a specified number of steps with a specified delay between each step.
 *
 * @param dx        The total horizontal distance to move the cursor.
 * @param steps     The number of incremental steps to break the total movement into (default is 100).
 * @param delay_us  The delay (in microseconds) between each incremental movement step (default is 500 microseconds).
 */
void moveMouse(int dx, int steps = 100, int delay_us = 500) {
    // Create an INPUT struct to hold the mouse movement event data.
    INPUT input;
    
    // Set the type of the INPUT struct to be a mouse event.
    input.type = INPUT_MOUSE;
    
    // Set the mouse event flag to indicate movement.
    input.mi.dwFlags = MOUSEEVENTF_MOVE;
    
    // Ensure there is no vertical movement of the cursor.
    input.mi.dy = 0;

    // Calculate how much the cursor should move horizontally in each step.
    int step_size = dx / steps;

    // Loop through each step, moving the mouse the specified amount and then waiting the specified delay.
    for (int i = 0; i < steps; ++i) {
        // Use the calculated step size for horizontal movement.
        input.mi.dx = step_size;
        
        // Send the mouse movement event.
        SendInput(1, &input, sizeof(INPUT));

        // Wait for the specified delay in microseconds before the next movement.
        std::this_thread::sleep_for(std::chrono::microseconds(delay_us)); 
    }
}


/**
 * Executes a sequence of mouse actions based on a specified mode.
 *
 * @param mode  A string that indicates the mode or pattern of actions to be taken.
 */
void executeMacro(const char* mode) {
    // Variables to hold the starting and ending time of specific actions for timing purposes.
    __int64 startTime, endTime;

    // Initial move of the mouse cursor to the left by 600 units.
    moveMouse(-600);
    // Wait for 500 milliseconds.
    waitMicroseconds(500000);

// Time the movement of the mouse cursor to the right by 600 units.
    startTime = GetCurrentTimeMicroseconds();
    moveMouse(600);
    endTime = GetCurrentTimeMicroseconds();
    // Print the duration of the move.
    PrintActionTime("Move right timer", startTime, endTime);

    // If the mode is "CS2", wait for 4 milliseconds and print the waiting duration.
    if (strcmp(mode, "CS2") == 0) {
        startTime = GetCurrentTimeMicroseconds();
        waitMicroseconds(4000);
        endTime = GetCurrentTimeMicroseconds();
        PrintActionTime("Wait timer", startTime, endTime);
    }

    // Time and execute a left click action.
    startTime = GetCurrentTimeMicroseconds();
    leftClick();
    endTime = GetCurrentTimeMicroseconds();
    // Print the duration of the left click.
    PrintActionTime("Left click timer", startTime, endTime);

    // If the mode is "CSGO", wait for 95 milliseconds.
    if (strcmp(mode, "CSGO") == 0) {
        waitMicroseconds(95000);
    }

    // Time the movement of the mouse cursor to the right by 600 units.
    startTime = GetCurrentTimeMicroseconds();
    moveMouse(600);
    endTime = GetCurrentTimeMicroseconds();
    // Print the duration of the move.
    PrintActionTime("Move right timer", startTime, endTime);

    // Wait for 1 second.
    waitMicroseconds(1000000);
    // Move the mouse cursor to the left by 600 units.
    moveMouse(-600);
}


int main() {
    std::cout << "" << std::endl;
    std::cout << "	===================================" << std::endl;
    std::cout << "	Monitoring Keys" << std::endl;
    std::cout << "7 for CS2         (Delay before shot)" << std::endl;
    std::cout << "8 for Diabotical  (no delays)" << std::endl;
    std::cout << "9 for CSGO        (Delay after shot)" << std::endl;
    std::cout << "\tPress Ctrl+P to exit." << std::endl;
    std::cout << "	===================================" << std::endl;
    std::cout << "" << std::endl;
    
    while (true) {
        if (isKeyPressed('7')) {
            executeMacro("CS2");
                    std::cout << "" << std::endl;
                    std::cout << "	===================================" << std::endl;
                    std::cout << "	Monitoring Keys" << std::endl;
                    std::cout << "7 for CS2         (Delay before shot)" << std::endl;
                    std::cout << "8 for Diabotical  (no delays)" << std::endl;
                    std::cout << "9 for CSGO        (Delay after shot)" << std::endl;
                    std::cout << "\tPress Ctrl+P to exit." << std::endl;
                    std::cout << "	===================================" << std::endl;
                    std::cout << "" << std::endl;
        } else if (isKeyPressed('8')) {
            executeMacro("Diabotical");
                    std::cout << "" << std::endl;
                    std::cout << "	===================================" << std::endl;
                    std::cout << "	Monitoring Keys" << std::endl;
                    std::cout << "7 for CS2         (Delay before shot)" << std::endl;
                    std::cout << "8 for Diabotical  (no delays)" << std::endl;
                    std::cout << "9 for CSGO        (Delay after shot)" << std::endl;
                    std::cout << "\tPress Ctrl+P to exit." << std::endl;
                    std::cout << "	===================================" << std::endl;
                    std::cout << "" << std::endl;
        } else if (isKeyPressed('9')) {
            executeMacro("CSGO");
                    std::cout << "" << std::endl;
                    std::cout << "	===================================" << std::endl;
                    std::cout << "	Monitoring Keys" << std::endl;
                    std::cout << "7 for CS2         (Delay before shot)" << std::endl;
                    std::cout << "8 for Diabotical  (no delays)" << std::endl;
                    std::cout << "9 for CSGO        (Delay after shot)" << std::endl;
                    std::cout << "\tPress Ctrl+P to exit." << std::endl;
                    std::cout << "	===================================" << std::endl;
                    std::cout << "" << std::endl;
        }

        if (isKeyPressed(VK_CONTROL) && isKeyPressed('P')) {
            std::cout << "Ctrl + P is pressed. Exiting the program." << std::endl;
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return 0;
}