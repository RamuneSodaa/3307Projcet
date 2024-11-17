#ifndef PREREQUISITECHECKER_H
#define PREREQUISITECHECKER_H

#include "Student.h"
#include "Course.h"

class PrerequisiteChecker {
public:
    // Constructor and Destructor
    PrerequisiteChecker() = default;
    ~PrerequisiteChecker() = default;

    // Check if a student meets the prerequisites for a course
    bool verifyPrerequisites(const Student* student, const Course* course) const;

    // Display missing prerequisites for a course
    void displayMissingPrerequisites(const Student* student, const Course* course) const;

    // NEW: Notify the student about missing prerequisites
    void notifyStudentMissingPrerequisites(const Student* student, const Course* course) const;

    // NEW: Suggest courses to fulfill missing prerequisites
    void suggestFulfillmentCourses(const Student* student, const Course* course) const;
};

#endif // PREREQUISITECHECKER_H
