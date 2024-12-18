#include "PrerequisiteChecker.h"
#include <iostream>
#include <algorithm>

// Verify if a student meets the prerequisites for a course
// New goal: 100-level courses (course_id < 200) have no prerequisites
bool PrerequisiteChecker::verifyPrerequisites(const Student* student, const Course* course) const {
    if (course->getCourseID() < 200) {
        // For 100-level courses, no prerequisites are required
        return true;
    }

    const auto& prerequisites = course->getPrerequisites();
    const auto& registeredCourses = student->getRegisteredCourses();

    for (const auto& prereq : prerequisites) {
        bool met = false;
        for (const auto& registeredCourse : registeredCourses) {
            if (registeredCourse->getCourseID() == prereq->getCourseID()) {
                met = true;
                break;
            }
        }
        if (!met) {
            return false; // If any prerequisite is not met, return false
        }
    }
    return true; // All prerequisites are met
}

// Display the missing prerequisites
// Unchanged except that if it's a 100-level course, there will be no missing prerequisites.
void PrerequisiteChecker::displayMissingPrerequisites(const Student* student, const Course* course) const {
    if (course->getCourseID() < 200) {
        // No prerequisites for 100-level courses
        std::cout << "This is a 100-level course and requires no prerequisites.\n";
        return;
    }

    const auto& prerequisites = course->getPrerequisites();
    const auto& registeredCourses = student->getRegisteredCourses();

    std::cout << "Missing prerequisites for course " << course->getCourseName() << ":" << std::endl;

    for (const auto& prereq : prerequisites) {
        bool met = false;
        for (const auto& registeredCourse : registeredCourses) {
            if (registeredCourse->getCourseID() == prereq->getCourseID()) {
                met = true;
                break;
            }
        }
        if (!met) {
            std::cout << "- " << prereq->getCourseName() << std::endl;
        }
    }
}

// Notify the student about missing prerequisites
// Also unchanged, but now if the course is 100-level, it states no prerequisites.
void PrerequisiteChecker::notifyStudentMissingPrerequisites(const Student* student, const Course* course) const {
    if (course->getCourseID() < 200) {
        std::cout << "Notification for Student ID: " << student->getStudentID() << std::endl;
        std::cout << "This is a 100-level course and requires no prerequisites.\n";
        return;
    }

    std::cout << "Notification for Student ID: " << student->getStudentID() << std::endl;
    std::cout << "You do not meet the prerequisites for the course: " << course->getCourseName() << std::endl;
    displayMissingPrerequisites(student, course);
}

// Suggest courses to fulfill missing prerequisites
// Same logic, if it's a 100-level course, no suggestions needed
void PrerequisiteChecker::suggestFulfillmentCourses(const Student* student, const Course* course) const {
    if (course->getCourseID() < 200) {
        std::cout << "This 100-level course requires no prerequisites.\n";
        return;
    }

    const auto& prerequisites = course->getPrerequisites();
    const auto& registeredCourses = student->getRegisteredCourses();

    std::cout << "Suggested courses to fulfill prerequisites for " << course->getCourseName() << ":" << std::endl;

    for (const auto& prereq : prerequisites) {
        bool met = false;
        for (const auto& registeredCourse : registeredCourses) {
            if (registeredCourse->getCourseID() == prereq->getCourseID()) {
                met = true;
                break;
            }
        }
        if (!met) {
            std::cout << "- Consider taking: " << prereq->getCourseName() << " (ID: " << prereq->getCourseID() << ")" << std::endl;
        }
    }
}
