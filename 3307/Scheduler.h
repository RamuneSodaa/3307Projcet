#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include "Course.h"
#include "PrerequisiteChecker.h"
#include "Student.h" // Include Student for student-related operations

class Scheduler {
private:
    std::vector<Course*> courseOfferings; // List of available courses
    std::string semester; // Current semester
    PrerequisiteChecker prerequisiteChecker; // Integration with PrerequisiteChecker

public:
    // Constructor and Destructor
    Scheduler(const std::string& semester);
    ~Scheduler();

    // Getters
    std::vector<Course*> getCourseOfferings() const;

    // Course Management
    void addCourse(Course* course);
    void removeCourse(int courseID);
    void updateCourseDetails(int courseID, const std::string& schedule);

    // Scheduling and Enrollment
    bool scheduleCourse(Student* student, int courseID);
    void generateSchedule(Student* student) const;
    bool enrollFromDraft(Student* student); // NEW: Enrolls courses from the draft schedule
    void dropFromDraft(Student* student, int courseID); // NEW: Removes a course from the draft schedule

    // Optimization
    std::vector<Course*> optimizeSchedule(const std::vector<int>& desiredCourses);

    // Utility
    void displayCourseOfferings() const;
    void handleEnrollmentFailure(Student* student, Course* course); // NEW: Handles failures like prerequisites or full capacity
};

#endif // SCHEDULER_H
