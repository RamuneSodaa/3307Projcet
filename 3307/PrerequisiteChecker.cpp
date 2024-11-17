#include "PrerequisiteChecker.h"
#include <iostream>

// Verify if a student meets the prerequisites for a course
bool PrerequisiteChecker::verifyPrerequisites(const Student* student, const Course* course) const {
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
void PrerequisiteChecker::displayMissingPrerequisites(const Student* student, const Course* course) const {
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
