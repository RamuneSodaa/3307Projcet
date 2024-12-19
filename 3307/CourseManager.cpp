#include "CourseManager.h"
#include "DatabaseManager.h"
#include "Course.h"
#include <iostream>
#include <algorithm>
#include <cctype>

static std::string toLowerCase(const std::string& s) {
    std::string lower = s;
    std::transform(lower.begin(), lower.end(), lower.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return lower;
}

CourseManager::CourseManager() {
    // Load additional courses from the database
    loadCoursesFromDatabase();
    std::cout << "CourseManager initialized with default and DB courses.\n";
}

CourseManager::~CourseManager() {
    for (auto& course : courseList) {
        delete course;
    }
}

void CourseManager::loadCoursesFromDatabase() {
    std::vector<Course> dbCourses = DatabaseManager::getAllCourses();
    for (const auto& c : dbCourses) {
        // Check if course already exists to avoid duplicates
        bool exists = false;
        for (auto existing : courseList) {
            if (existing->getCourseID() == c.getCourseID()) {
                exists = true;
                break;
            }
        }
        if (!exists) {
            courseList.push_back(new Course(c.getCourseID(), c.getCourseName(), c.getSchedule(), c.getCapacity(), c.getPrereqStr()));
            std::cout << "Course loaded from DB: " << c.getCourseName() << " (ID: " << c.getCourseID() << ")\n";
        }
    }
}

void CourseManager::addCourse(Course* course) {
    if (!course) {
        std::cerr << "Cannot add a null course.\n";
        return;
    }
    courseList.push_back(course);
    std::cout << "Course added: " << course->getCourseName()
              << " (ID: " << course->getCourseID() << ")\n";

    DatabaseManager::addCourse(*course);
}

bool CourseManager::removeCourse(int courseID) {
    auto it = std::remove_if(courseList.begin(), courseList.end(),
                             [courseID](Course* course) {
                                 return course->getCourseID() == courseID;
                             });
    if (it != courseList.end()) {
        delete *it;
        courseList.erase(it, courseList.end());
        std::cout << "Course with ID " << courseID << " removed.\n";
        return true;
    }
    std::cout << "Course with ID " << courseID << " not found.\n";
    return false;
}

std::vector<Course*> CourseManager::searchCourses(const std::string& query) const {
    std::vector<Course*> results;
    std::string lowerQuery = toLowerCase(query);
    bool isNumeric = !lowerQuery.empty() && std::all_of(lowerQuery.begin(), lowerQuery.end(), ::isdigit);

    if (isNumeric) {
        int courseID = std::stoi(lowerQuery);
        for (auto& course : courseList) {
            if (course->getCourseID() == courseID) {
                results.push_back(course);
                return results;
            }
        }
        return results;
    } else {
        for (auto& course : courseList) {
            std::string courseNameLower = toLowerCase(course->getCourseName());
            if (courseNameLower.find(lowerQuery) != std::string::npos) {
                results.push_back(course);
            }
        }
        return results;
    }
}


void CourseManager::browseCourses() const {
    std::cout << "Available Courses:\n";
    for (const auto& course : courseList) {
        std::cout << "ID: " << course->getCourseID()
                  << ", Name: " << course->getCourseName()
                  << ", Schedule: " << course->getSchedule()
                  << ", Capacity: " << course->getCapacity() << "\n";
    }
}

void CourseManager::displayCourseDetails(int courseID) const {
    for (auto& course : courseList) {
        if (course->getCourseID() == courseID) {
            std::cout << "Course Details:\n";
            std::cout << "ID: " << course->getCourseID() << "\n";
            std::cout << "Name: " << course->getCourseName() << "\n";
            std::cout << "Schedule: " << course->getSchedule() << "\n";
            std::cout << "Capacity: " << course->getCapacity() << "\n";
            std::cout << "Prerequisites: ";
            for (auto& prereq : course->getPrerequisites()) {
                std::cout << prereq->getCourseName() << " ";
            }
            std::cout << "\n";
            return;
        }
    }
    std::cout << "No details available for Course ID: " << courseID << "\n";
}

std::vector<Course*> CourseManager::filterCoursesByDepartment(const std::string& department) const {
    std::vector<Course*> filteredCourses;
    for (const auto& course : courseList) {
        if (course->getCourseName().find(department) != std::string::npos) {
            filteredCourses.push_back(course);
        }
    }
    return filteredCourses;
}

const std::vector<Course*>& CourseManager::getCourseList() const {
    return courseList;
}
