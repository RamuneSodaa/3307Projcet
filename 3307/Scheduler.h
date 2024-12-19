#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include <string>
#include "PrerequisiteChecker.h"

class Student;
class Course;

class Scheduler {
public:
    Scheduler(const std::string& semester);
    ~Scheduler();

    void addCourse(Course* course);
    void removeCourse(int courseID);
    void updateCourseDetails(int courseID, const std::string& schedule);
    bool scheduleCourse(Student* student, int courseID);
    void generateSchedule(Student* student) const;
    bool enrollFromDraft(Student* student);
    void dropFromDraft(Student* student, int courseID);
    std::vector<Course*> optimizeSchedule(const std::vector<int>& desiredCourses);
    void displayCourseOfferings() const;
    std::vector<std::string> validateDraftSchedule(Student* student);
    bool finalizeDraftSchedule(Student* student);

private:
    std::string semester;
    std::vector<Course*> courseOfferings;
    PrerequisiteChecker prerequisiteChecker;

    void handleEnrollmentFailure(Student* student, Course* course);
};

#endif // SCHEDULER_H
