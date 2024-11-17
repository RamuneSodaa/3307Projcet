#include "Student.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

// Constructor
Student::Student(int id, const std::string& uname, const std::string& email, const std::string& status)
    : studentID(id), username(uname), email(email), status(status) {}

// Destructor
Student::~Student() {
    registeredCourses.clear(); // Do not delete courses, as Scheduler owns them
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

// Add a course to the student's schedule
void Student::addCourse(Course* course) {
    if (!course) {
        std::cerr << "Cannot add a null course to the schedule.\n";
        return;
    }
    registeredCourses.push_back(course); // Add reference only
    logActivity("Added course: " + course->getCourseName());
    std::cout << "Course " << course->getCourseName() << " added for student " << username << ".\n";
}

// Drop a course from the student's schedule
void Student::dropCourse(int courseID) {
    auto it = std::remove_if(registeredCourses.begin(), registeredCourses.end(),
                             [courseID](Course* course) { return course->getCourseID() == courseID; });
    if (it != registeredCourses.end()) {
        registeredCourses.erase(it, registeredCourses.end());
        logActivity("Dropped course with ID: " + std::to_string(courseID));
        std::cout << "Course dropped successfully.\n";
    } else {
        std::cout << "Course with ID " << courseID << " not found in the schedule.\n";
    }
}

// List all registered courses
void Student::listCourses() const {
    if (registeredCourses.empty()) {
        std::cout << "No courses registered for student " << username << ".\n";
        return;
    }

    std::cout << "Schedule for Student ID: " << studentID << "\n";
    for (const auto& course : registeredCourses) {
        std::cout << course->getCourseName() << " - " << course->getSchedule() << "\n";
    }
}

// Log activity to the student's activity log
void Student::logActivity(const std::string& activityDescription) {
    std::string timestampedLog = getCurrentTimestamp() + " - " + activityDescription;
    activityLog.push_back(timestampedLog);

    // Log to a file
    std::ofstream logFile("activity_log.txt", std::ios::app);
    if (logFile.is_open()) {
        logFile << "Student ID: " << studentID << " | " << timestampedLog << "\n";
        logFile.close();
    } else {
        std::cerr << "Failed to write to activity log file.\n";
    }
}

// Show activity log
void Student::showActivityLog() const {
    if (activityLog.empty()) {
        std::cout << "No activities logged for student " << username << ".\n";
        return;
    }

    std::cout << "Activity log for student " << username << ":\n";
    for (const auto& log : activityLog) {
        std::cout << log << "\n";
    }
}

// Helper: Get the current timestamp
std::string Student::getCurrentTimestamp() const {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}
