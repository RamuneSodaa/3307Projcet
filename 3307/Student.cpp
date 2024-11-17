#include "Student.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <algorithm>

// Constructor
Student::Student(int id, const std::string& uname, const std::string& email, const std::string& status)
    : studentID(id), username(uname), email(email), status(status) {}

// Destructor
Student::~Student() {
    for (Course* course : registeredCourses) {
        delete course;
    }
    registeredCourses.clear();
}

// Getters
int Student::getStudentID() const {
    return studentID;
}

std::string Student::getUsername() const {
    return username;
}

std::string Student::getEmail() const {
    return email;
}

std::string Student::getStatus() const {
    return status;
}

const std::vector<Course*>& Student::getRegisteredCourses() const {
    return registeredCourses;
}

// Setters
void Student::setStatus(const std::string& newStatus) {
    status = newStatus;
}

// Add a course
void Student::addCourse(Course* course) {
    registeredCourses.push_back(course);
    logActivity("Added course: " + course->getCourseName());
    std::cout << "Course " << course->getCourseName() << " added for student " << username << "." << std::endl;
}

// Drop a course
void Student::dropCourse(int courseID) {
    auto it = std::remove_if(registeredCourses.begin(), registeredCourses.end(),
                             [courseID](Course* course) {
                                 if (course->getCourseID() == courseID) {
                                     delete course;
                                     return true;
                                 }
                                 return false;
                             });

    if (it != registeredCourses.end()) {
        registeredCourses.erase(it, registeredCourses.end());
        logActivity("Dropped course with ID: " + std::to_string(courseID));
        std::cout << "Course with ID " << courseID << " dropped for student " << username << "." << std::endl;
    } else {
        std::cout << "Course with ID " << courseID << " not found for student " << username << "." << std::endl;
    }
}

// List all registered courses
void Student::listCourses() const {
    std::cout << "Registered courses for " << username << " (ID: " << studentID << "):" << std::endl;
    for (const Course* course : registeredCourses) {
        std::cout << "- " << course->getCourseName() << " (" << course->getSchedule() << ")" << std::endl;
    }
}

// Helper: Get current timestamp
std::string Student::getCurrentTimestamp() const {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

// Log activity
void Student::logActivity(const std::string& activityDescription) {
    std::string timestampedLog = getCurrentTimestamp() + " - " + activityDescription;
    activityLog.push_back(timestampedLog);

    // Log to file
    std::ofstream logFile("activity_log.txt", std::ios::app);
    if (logFile.is_open()) {
        logFile << "Student ID: " << studentID << " | " << timestampedLog << std::endl;
        logFile.close();
    }

    std::cout << "Activity logged: " << timestampedLog << std::endl;
}

// Show activity log
void Student::showActivityLog() const {
    std::cout << "Activity log for student " << username << " (ID: " << studentID << "):" << std::endl;
    for (const std::string& log : activityLog) {
        std::cout << log << std::endl;
    }
}
