#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include "Course.h"
#include "PrerequisiteChecker.h"
class Scheduler {
private:
    std::vector<Course*> courseOfferings;
    std::string semester;
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

    // Optimization
    std::vector<Course*> optimizeSchedule(const std::vector<int>& desiredCourses);

    // Utility
    void displayCourseOfferings() const;
};

#endif // SCHEDULER_H
