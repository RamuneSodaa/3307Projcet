#include "CourseManager.h"
#include <iostream>
#include <algorithm>

// Constructor
CourseManager::CourseManager() {}

// Destructor
CourseManager::~CourseManager() {
    for (auto& course : courseList) {
        delete course; // Clean up dynamically allocated courses
    }
}

// Add a course to the system
void CourseManager::addCourse(Course* course) {
    courseList.push_back(course);
    std::cout << "Course added: " << course->getCourseName() << " (ID: " << course->getCourseID() << ")" << std::endl;
}

// Remove a course from the system by ID
bool CourseManager::removeCourse(int courseID) {
    auto it = std::remove_if(courseList.begin(), courseList.end(),
        [courseID](Course* course) { return course->getCourseID() == courseID; });

    if (it != courseList.end()) {
        delete *it; // Free the memory of the removed course
        courseList.erase(it, courseList.end());
        std::cout << "Course with ID " << courseID << " removed." << std::endl;
        return true;
    }
    std::cout << "Course with ID " << courseID << " not found." << std::endl;
    return false;
}

// Search for a course by ID
Course* CourseManager::searchCourse(int courseID) const {
    for (auto& course : courseList) {
        if (course->getCourseID() == courseID) {
            return course;
        }
    }
    return nullptr; // Course not found
}

// Display all available courses
void CourseManager::browseCourses() const {
    std::cout << "Available Courses:" << std::endl;
    for (const auto& course : courseList) {
        std::cout << "ID: " << course->getCourseID()
                  << ", Name: " << course->getCourseName()
                  << ", Schedule: " << course->getSchedule()
                  << ", Capacity: " << course->getCapacity() << std::endl;
    }
}

// Display details for a specific course by ID
void CourseManager::displayCourseDetails(int courseID) const {
    Course* course = searchCourse(courseID);
    if (course) {
        std::cout << "Course Details:" << std::endl;
        std::cout << "ID: " << course->getCourseID() << std::endl;
        std::cout << "Name: " << course->getCourseName() << std::endl;
        std::cout << "Schedule: " << course->getSchedule() << std::endl;
        std::cout << "Capacity: " << course->getCapacity() << std::endl;
        std::cout << "Prerequisites: ";
        for (auto& prereq : course->getPrerequisites()) {
            std::cout << prereq->getCourseName() << " ";
        }
        std::cout << std::endl;
    } else {
        std::cout << "No details available for Course ID: " << courseID << std::endl;
    }
}

// Filter courses by department (optional feature for future)
std::vector<Course*> CourseManager::filterCoursesByDepartment(const std::string& department) const {
    std::vector<Course*> filteredCourses;
    for (const auto& course : courseList) {
        if (course->getCourseName().find(department) != std::string::npos) { // Example filter logic
            filteredCourses.push_back(course);
        }
    }
    return filteredCourses;
}
