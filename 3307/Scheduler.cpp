#include "Scheduler.h"
#include <iostream>
#include <stdexcept>

// Constructor
Scheduler::Scheduler(const std::string& semester) : semester(semester) {}

// Destructor
Scheduler::~Scheduler() {
    // Do not delete objects that are not dynamically allocated
    courseOfferings.clear(); // Clear the vector to ensure its own memory is released
}

// Getters
std::vector<Course*> Scheduler::getCourseOfferings() const {
    return courseOfferings;
}

// Course Management
void Scheduler::addCourse(Course* course) {
    courseOfferings.push_back(course);
    std::cout << "Course added: " << course->getCourseName() << std::endl;
}

void Scheduler::removeCourse(int courseID) {
    for (auto it = courseOfferings.begin(); it != courseOfferings.end(); ++it) {
        if ((*it)->getCourseID() == courseID) {
            courseOfferings.erase(it);
            std::cout << "Course removed: " << courseID << std::endl;
            return;
        }
    }
    throw std::invalid_argument("Course not found!");
}

void Scheduler::updateCourseDetails(int courseID, const std::string& schedule) {
    for (auto& course : courseOfferings) {
        if (course->getCourseID() == courseID) {
            course->setSchedule(schedule);
            std::cout << "Course schedule updated for: " << courseID << std::endl;
            return;
        }
    }
    throw std::invalid_argument("Course not found!");
}

// Scheduling and Enrollment
bool Scheduler::scheduleCourse(Student* student, int courseID) {
    for (auto& course : courseOfferings) {
        if (course->getCourseID() == courseID) {
            // Prerequisite validation
            if (!prerequisiteChecker.verifyPrerequisites(student, course)) {
                std::cout << "Prerequisite check failed for course: " << course->getCourseName() << std::endl;
                prerequisiteChecker.displayMissingPrerequisites(student, course);
                return false;
            }

            // Enrollment
            if (course->enrollStudent(student)) {
                student->addCourse(course);
                std::cout << "Student enrolled in course: " << course->getCourseName() << std::endl;
                return true;
            } else {
                std::cout << "Course is full. Added to waitlist: " << courseID << std::endl;
                return false;
            }
        }
    }
    throw std::invalid_argument("Course not found!");
}

void Scheduler::generateSchedule(Student* student) const {
    std::cout << "Schedule for Student ID: " << student->getStudentID() << std::endl;
    for (const auto& course : student->getRegisteredCourses()) {
        std::cout << course->getCourseName() << " - " << course->getSchedule() << std::endl;
    }
}

// Optimization
std::vector<Course*> Scheduler::optimizeSchedule(const std::vector<int>& desiredCourses) {
    std::vector<Course*> optimizedCourses;
    for (int id : desiredCourses) {
        for (auto& course : courseOfferings) {
            if (course->getCourseID() == id) {
                optimizedCourses.push_back(course);
                break;
            }
        }
    }
    return optimizedCourses;
}

// Utility
void Scheduler::displayCourseOfferings() const {
    std::cout << "Available Courses: " << std::endl;
    for (const auto& course : courseOfferings) {
        std::cout << "Course ID: " << course->getCourseID()
                  << ", Name: " << course->getCourseName()
                  << ", Schedule: " << course->getSchedule()
                  << ", Capacity: " << course->getCapacity() << std::endl;
    }
}
