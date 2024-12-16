#include <iostream>
#include "Authentication.h"
#include "CourseManager.h"
#include "Scheduler.h"
#include "Student.h"
#include "MainInterface.h" // Includes declarations for showLoginPage() and showMainMenu()

// Define global variables once here
Student* currentStudent = nullptr;
Authentication* auth = Authentication::getInstance();
CourseManager courseManager;
Scheduler scheduler("Fall 2024");

int main() {
    // Add a test user
    auth->addUser("XFENG282", "password123");

    // Show the login page (implemented in MainInterface.cpp)
    bool loginSuccess = showLoginPage(); // Must be declared in MainInterface.h

    if (loginSuccess && currentStudent) {
        // If the user logged in successfully, show the main menu
        if (!showMainMenu()) {
            std::cerr << "Failed to show main menu.\n";
            return -1;
        }
    } else {
        std::cout << "Login failed or user exited.\n";
    }

    return 0;
}
