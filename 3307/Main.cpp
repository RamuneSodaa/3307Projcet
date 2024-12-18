#include <iostream>
#include "Authentication.h"
#include "CourseManager.h"
#include "Scheduler.h"
#include "Student.h"
#include "DatabaseManager.h"
#include "LoginInterface.h"
#include "MainMenuInterface.h"

Student* currentStudent=nullptr;
Authentication* auth=Authentication::getInstance();
CourseManager courseManager;
Scheduler scheduler("Fall 2024");

int main() {
    DatabaseManager::initializeDatabase("myDatabase.db", "../init_db.sql");

    auth->addUser("1","2");
    if(LoginInterface().show() && currentStudent) {
        MainMenuInterface().show();
    } else {
        std::cout<<"Login failed or user exited.\n";
    }
    return 0;
}
