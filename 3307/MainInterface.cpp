#include "MainInterface.h"
#include "Authentication.h"
#include "CourseManager.h"
#include "Scheduler.h"
#include "Student.h"
#include <iostream>
#include <string>

// Shared objects
Authentication* auth = Authentication::getInstance();
CourseManager courseManager;
Scheduler scheduler("Fall 2024");
Student* currentStudent = nullptr;

// Login functionality
void login() {
    std::string username, password;
    std::cout << "\n--- Login ---\n";
    std::cout << "Username: ";
    std::cin >> username;
    std::cout << "Password: ";
    std::cin >> password;

    if (auth->login(username, password)) {
        currentStudent = new Student(1, username, username + "@example.com", "active");
        std::cout << "Login successful! Welcome, " << username << ".\n";
    } else {
        std::cout << "Invalid credentials. Please try again.\n";
    }
}

// Browse all courses
void browseCourses() {
    std::cout << "\n--- Browse Courses ---\n";
    courseManager.browseCourses(); // Directly calls CourseManager's browseCourses method
}


// Search courses by keyword
void searchCourses() {
    std::string keyword;
    std::cout << "\n--- Search Courses ---\n";
    std::cout << "Enter keyword: ";
    std::cin >> keyword;

    auto results = courseManager.filterCoursesByDepartment(keyword);
    if (results.empty()) {
        std::cout << "No courses found for \"" << keyword << "\".\n";
    } else {
        std::cout << "Courses matching \"" << keyword << "\":\n";
        for (const auto& course : results) {
            std::cout << "ID: " << course->getCourseID()
                      << ", Name: " << course->getCourseName()
                      << ", Schedule: " << course->getSchedule()
                      << ", Capacity: " << course->getCapacity() << std::endl;
        }
    }
}

// View student's profile
void viewProfile() {
    if (!currentStudent) {
        std::cout << "No student logged in.\n";
        return;
    }

    std::cout << "\n--- Student Profile ---\n";
    std::cout << "ID: " << currentStudent->getStudentID() << "\n";
    std::cout << "Name: " << currentStudent->getUsername() << "\n";
    std::cout << "Email: " << currentStudent->getEmail() << "\n";
    currentStudent->listCourses();
}

// Manage enrollment
void manageEnrollment() {
    if (!currentStudent) {
        std::cout << "No student logged in.\n";
        return;
    }

    int choice;
    std::cout << "\n--- Manage Enrollment ---\n";
    std::cout << "1. Enroll in a course\n";
    std::cout << "2. Drop a course\n";
    std::cout << "3. View draft schedule\n";
    std::cout << "4. Finalize schedule\n";
    std::cin >> choice;

    if (choice == 1) {
        int courseID;
        std::cout << "Enter Course ID to enroll: ";
        std::cin >> courseID;
        if (scheduler.scheduleCourse(currentStudent, courseID)) {
            std::cout << "Enrollment successful.\n";
        } else {
            std::cout << "Enrollment failed.\n";
        }
    } else if (choice == 2) {
        int courseID;
        std::cout << "Enter Course ID to drop: ";
        std::cin >> courseID;
        currentStudent->dropCourse(courseID);
    } else if (choice == 3) {
        scheduler.generateSchedule(currentStudent);
    } else if (choice == 4) {
        if (scheduler.finalizeDraftSchedule(currentStudent)) {
            std::cout << "Schedule finalized successfully.\n";
        } else {
            std::cout << "Schedule finalization failed.\n";
        }
    } else {
        std::cout << "Invalid choice.\n";
    }
}

// Main menu
bool showMainMenu() {
    if (!currentStudent) {
        std::cout << "Please log in first.\n";
        return false;
    }

    int choice;
    do {
        std::cout << "\n=== Main Menu ===\n";
        std::cout << "1. Browse Courses\n";
        std::cout << "2. Search Courses\n";
        std::cout << "3. View Profile\n";
        std::cout << "4. Manage Enrollment\n";
        std::cout << "5. Logout\n";
        std::cin >> choice;

        switch (choice) {
            case 1: browseCourses(); break;
            case 2: searchCourses(); break;
            case 3: viewProfile(); break;
            case 4: manageEnrollment(); break;
            case 5: 
                std::cout << "Logging out...\n"; 
                delete currentStudent; 
                currentStudent = nullptr;
                return true;
            default:
                std::cout << "Invalid choice. Try again.\n";
        }
    } while (true);
}
