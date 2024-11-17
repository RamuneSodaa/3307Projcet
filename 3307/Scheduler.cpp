#include "Scheduler.h"
#include <iostream>
#include <algorithm>

// Constructor
Scheduler::Scheduler(const std::string& semester) : semester(semester) {}

// Destructor
Scheduler::~Scheduler() {
    for (auto course : courseOfferings) {
        delete course; // Ensure proper cleanup
    }
}

// Getters
std::vector<Course*> Scheduler::getCourseOfferings() const {
    return courseOfferings;
}

// Add a course to the offerings
void Scheduler::addCourse(Course* course) {
    if (!course) {
        std::cerr << "Cannot add a null course.\n";
        return;
    }
    courseOfferings.push_back(course);
    std::cout << "Course added: " << course->getCourseName() << "\n";
}

// Remove a course by ID
void Scheduler::removeCourse(int courseID) {
    auto it = std::remove_if(courseOfferings.begin(), courseOfferings.end(),
                             [courseID](Course* course) { return course->getCourseID() == courseID; });
    if (it != courseOfferings.end()) {
        delete *it; // Free memory
        courseOfferings.erase(it);
        std::cout << "Course removed: " << courseID << "\n";
    } else {
        std::cerr << "Course not found with ID: " << courseID << "\n";
    }
}

// Update course details
void Scheduler::updateCourseDetails(int courseID, const std::string& schedule) {
    for (auto course : courseOfferings) {
        if (course->getCourseID() == courseID) {
            course->setSchedule(schedule);
            std::cout << "Course ID " << courseID << " updated with new schedule: " << schedule << "\n";
            return;
        }
    }
    std::cerr << "Course not found with ID: " << courseID << "\n";
}

// Schedule a course for a student
bool Scheduler::scheduleCourse(Student* student, int courseID) {
    if (!student) {
        std::cerr << "Student object cannot be null.\n";
        return false;
    }

    for (auto course : courseOfferings) {
        if (course->getCourseID() == courseID) {
            // Check prerequisites
            if (!prerequisiteChecker.verifyPrerequisites(student, course)) {
                std::cout << "Prerequisite check failed for course: " << course->getCourseName() << "\n";
                prerequisiteChecker.displayMissingPrerequisites(student, course);
                return false;
            }

            // Enroll the student
            if (course->enrollStudent(student)) {
                student->addCourse(course);
                std::cout << "Student enrolled in course: " << course->getCourseName() << "\n";
                return true;
            } else {
                std::cout << "Course is full.\n";
                return false;
            }
        }
    }
    std::cerr << "Course not found with ID: " << courseID << "\n";
    return false;
}

// Generate the student's schedule
void Scheduler::generateSchedule(Student* student) const {
    if (!student) {
        std::cerr << "Student object cannot be null.\n";
        return;
    }

    std::cout << "Schedule for Student ID: " << student->getStudentID() << "\n";
    for (const auto& course : student->getRegisteredCourses()) {
        std::cout << course->getCourseName() << " - " << course->getSchedule() << "\n";
    }
}

// Optimize the student's schedule
std::vector<Course*> Scheduler::optimizeSchedule(const std::vector<int>& desiredCourses) {
    std::vector<Course*> optimizedCourses;
    for (int courseID : desiredCourses) {
        for (const auto& course : courseOfferings) {
            if (course->getCourseID() == courseID) {
                optimizedCourses.push_back(course);
                break;
            }
        }
    }
    return optimizedCourses;
}

// Display all available courses
void Scheduler::displayCourseOfferings() const {
    if (courseOfferings.empty()) {
        std::cout << "No courses available.\n";
        return;
    }

    std::cout << "Available Courses:\n";
    for (const auto& course : courseOfferings) {
        std::cout << "Course ID: " << course->getCourseID()
                  << ", Name: " << course->getCourseName()
                  << ", Schedule: " << course->getSchedule()
                  << ", Capacity: " << course->getCapacity() << "\n";
    }
}
