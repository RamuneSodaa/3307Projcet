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

// Define extern variables once here
Student* currentStudent = nullptr;
Authentication* auth = Authentication::getInstance();

int main() {
    DatabaseManager::initializeDatabase("database.sqlite", "init_db.sql");

    CourseManager cm;
    Scheduler sched("Spring 2024");
    PrerequisiteChecker checker;
    EnrollmentManager em(cm, sched, checker);

    Student s1(1, "student1", "student1@example.com", "active");
    Student s2(2, "student2", "student2@example.com", "active");
    DatabaseManager::addStudent(s1);
    DatabaseManager::addStudent(s2);

    auth->addUser("1", "1");
    auth->addUser("2", "2");

    // Courses with numeric prereqs
    // Math I = 101 (no prereq)
    // Math II = 201 (prereq = 101)
    // Biology I = 102 (no prereq)
    // Biology II = 202 (prereq = 102)
    Course c1(101, "Math I", "Monday", 40);
    Course c2(201, "Math II", "Monday", 40, "101");
    Course c3(102, "Biology I", "Monday", 40);
    Course c4(202, "Biology II", "Monday", 40, "102");
    DatabaseManager::addCourse(c1);
    DatabaseManager::addCourse(c2);
    DatabaseManager::addCourse(c3);
    DatabaseManager::addCourse(c4);

    cm.loadCoursesFromDatabase();

    // Resolve prerequisites for all courses before enrollment checks
    for (auto c : cm.getCourseList()) {
        c->resolvePrerequisites(cm.getCourseList());
    }

    for (auto c : cm.getCourseList()) {
        sched.addCourse(c);
    }

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
