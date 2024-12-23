#include "Student.h"
#include "Course.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <algorithm>

/**
 * Constructs a Student object with detailed parameters.
 *
 * @param id The student's ID.
 * @param uname The username of the student.
 * @param email The email address of the student.
 * @param status The current status of the student.
 */
Student::Student(int id, const std::string& uname, const std::string& email, const std::string& status)
    : studentID(id), username(uname), email(email), status(status) {}

/**
 * Constructs a Student object from a string ID and email.
 * If the ID is not a valid integer, it defaults to 0.
 *
 * @param id The student's ID as a string.
 * @param email The email address of the student.
 */
Student::Student(const std::string& id, const std::string& email) {
    try {
        studentID = std::stoi(id);
    } catch (const std::invalid_argument& e) {
        std::cerr << "Invalid student ID: " << id << ". Defaulting to 0." << std::endl;
        studentID = 0; // Default to 0 if the conversion fails
    }
    username = "default";
    this->email = email;
    status = "active";
}

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

std::vector<Course*>& Student::getDraftSchedule() {
    return draftSchedule;
}

// Setters
void Student::setStatus(const std::string& newStatus) {
    status = newStatus;
}

/**
 * Finalizes the enrollment for a given course, moving it from the draft to the registered courses.
 *
 * @param course Pointer to the course to finalize enrollment in.
 */
void Student::finalizeEnrollment(Course* course) {
    if (!course) {
        std::cerr << "Cannot finalize enrollment for a null course.\n";
        return;
    }

    // Add course to registered courses
    registeredCourses.push_back(course);

    // Remove course from draft schedule
    auto it = std::remove(draftSchedule.begin(), draftSchedule.end(), course);
    if (it != draftSchedule.end()) {
        draftSchedule.erase(it);
    }

    std::cout << "Successfully finalized enrollment for course: " << course->getCourseName() << "\n";
    logActivity("Finalized enrollment for course: " + course->getCourseName());
}

void Student::clearDraftSchedule() {
    draftSchedule.clear();
    std::cout << "Draft schedule cleared for student " << username << ".\n";
}

/**
 * Adds a course to the student's registered courses.
 *
 * @param course Pointer to the course to add.
 */
void Student::addCourse(Course* course) {
    if (!course) {
        std::cerr << "Cannot add a null course to the schedule.\n";
        return;
    }
    registeredCourses.push_back(course); // Add reference only
    logActivity("Added course: " + course->getCourseName());
    std::cout << "Course " << course->getCourseName() << " added for student " << username << ".\n";
}

/**
 * Adds a course to the student's draft schedule.
 *
 * @param course Pointer to the course to add.
 */
void Student::addToDraftSchedule(Course* course) {
    if (!course) {
        std::cerr << "Cannot add a null course to the draft schedule.\n";
        return;
    }
    draftSchedule.push_back(course);
    logActivity("Added to draft schedule: " + course->getCourseName());
    std::cout << "Course " << course->getCourseName() << " added to draft schedule for student " << username << ".\n";
}

/**
 * Drops a course from the student's schedule by course ID.
 *
 * @param courseID The ID of the course to drop.
 */
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

/**
 * Drops a course from the student's draft schedule by course ID.
 *
 * @param courseID The ID of the course to drop.
 */
void Student::dropFromDraftSchedule(int courseID) {
    auto it = std::remove_if(draftSchedule.begin(), draftSchedule.end(),
                             [courseID](Course* course) { return course->getCourseID() == courseID; });
    if (it != draftSchedule.end()) {
        draftSchedule.erase(it, draftSchedule.end());
        logActivity("Dropped from draft schedule: " + std::to_string(courseID));
        std::cout << "Course removed from draft schedule successfully.\n";
    } else {
        std::cout << "Course with ID " << courseID << " not found in the draft schedule.\n";
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

// Additional method: remove a specific course pointer
void Student::removeCourse(Course* course) {
    if (!course) return;
    auto it = std::remove(registeredCourses.begin(), registeredCourses.end(), course);
    if (it != registeredCourses.end()) {
        registeredCourses.erase(it, registeredCourses.end());
        logActivity("Removed course: " + course->getCourseName());
    }
}

// Log activity
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
