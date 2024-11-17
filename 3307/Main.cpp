#include <iostream>
#include "Authentication.h"
#include "CourseManager.h"
#include "Scheduler.h"
#include "Student.h"
#include "PrerequisiteChecker.h"

int main() {
    // Authentication setup
    Authentication* auth = Authentication::getInstance();
    auth->addUser("student1", "password123");
    auth->addUser("student2", "securePass456");

    std::string username, password;
    std::cout << "Enter username: ";
    std::cin >> username;
    std::cout << "Enter password: ";
    std::cin >> password;

    if (!auth->login(username, password)) {
        std::cerr << "Authentication failed. Exiting...\n";
        return 1;
    }

    // Create Student
    Student student1("S12345", "student1@example.com");

    // CourseManager setup
    CourseManager courseManager;

    // Create courses
    Course* math101 = new Course(101, "Math 101", "Monday 10:00-12:00", 30);
    Course* physics102 = new Course(102, "Physics 102", "Tuesday 14:00-16:00", 25);
    Course* calculus201 = new Course(201, "Calculus 201", "Wednesday 10:00-12:00", 20);

    // Set prerequisites
    calculus201->addPrerequisite(math101);

    // Add courses to CourseManager
    courseManager.addCourse(math101);
    courseManager.addCourse(physics102);
    courseManager.addCourse(calculus201);

    // Display available courses
    courseManager.browseCourses();

    // Scheduler setup
    Scheduler scheduler("Fall 2024");

    scheduler.addCourse(math101);
    scheduler.addCourse(physics102);
    scheduler.addCourse(calculus201);

    // Add to draft schedule
    student1.addToDraftSchedule(math101);
    student1.addToDraftSchedule(calculus201);

    // Enroll in courses
    std::cout << "Enrolling in courses from draft schedule...\n";
    scheduler.enrollFromDraft(&student1);

    // Display student schedule
    std::cout << "\nFinalized Schedule:\n";
    scheduler.generateSchedule(&student1);

    // PrerequisiteChecker demo
    PrerequisiteChecker prereqChecker;
    if (!prereqChecker.verifyPrerequisites(&student1, calculus201)) {
        prereqChecker.displayMissingPrerequisites(&student1, calculus201);
        prereqChecker.suggestFulfillmentCourses(&student1, calculus201);
    }

    // Logout
    auth->logout(username);

    // Cleanup dynamically allocated memory
    delete math101;
    delete physics102;
    delete calculus201;

    return 0;
}
