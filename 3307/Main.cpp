#include "Authentication.h"
#include "Student.h"
#include "DatabaseManager.h"
#include "CourseManager.h"
#include "Scheduler.h"
#include "PrerequisiteChecker.h"
#include "EnrollmentManager.h"
#include "LoginInterface.h"
#include "MainMenuInterface.h"
#include <iostream>

// Global variables
Authentication* auth = Authentication::getInstance();
Student* currentStudent = nullptr;

int main() {
    // Initialize the database
    DatabaseManager::initializeDatabase("database.sqlite", "init_db.sql");

    CourseManager cm;
    Scheduler sched("Spring 2024");
    PrerequisiteChecker checker;
    EnrollmentManager em(cm, sched, checker);

    // Add some sample students to the database
    Student s1(1, "student1", "student1@example.com", "active");
    Student s2(2, "student2", "student2@example.com", "active");
    DatabaseManager::addStudent(s1);
    DatabaseManager::addStudent(s2);

    // Add user credentials for them
    auth->addUser("1", "1");
    auth->addUser("2", "2");

    // Add a new course to the database BEFORE loading them into CourseManager
    // This ensures it's present when we call loadCoursesFromDatabase().
    Course c1(101, "Math I", "Monday", 40);
    Course c2(201, "Math II", "Monday", 40,"Math I");
    DatabaseManager::addCourse(c1);
    DatabaseManager::addCourse(c2);

    // Now load courses from the DB into CourseManager's courseList
    cm.loadCoursesFromDatabase();

    // After loading courses into CourseManager, add them to the Scheduler
    for (auto c : cm.getCourseList()) {
        sched.addCourse(c);
    }

    // Show login interface
    LoginInterface login;
    if (login.show()) {
        std::cout << "Login successful. Welcome: " << currentStudent->getUsername() << "\n";

        MainMenuInterface mainMenu(cm, sched, em);
        mainMenu.show();
    } else {
        std::cout << "Login failed or user exited.\n";
    }

    return 0;
}
