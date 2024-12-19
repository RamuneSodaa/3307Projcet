#ifndef ENROLLMENTMANAGER_H
#define ENROLLMENTMANAGER_H

#include <string>

class CourseManager;
class Scheduler;
class PrerequisiteChecker;
class Student;

class EnrollmentManager {
public:
    EnrollmentManager(CourseManager& cm, Scheduler& sched, PrerequisiteChecker& checker);

    bool enrollStudentInCourse(Student* student, int courseID);
    bool addEnrollment(int studentID, int courseID, const std::string& courseName, const std::string& enrollmentDate);

    // Ensure this declaration exists:
    bool dropEnrollment(Student* student, int courseID);

private:
    CourseManager& courseManager;
    Scheduler& scheduler;
    PrerequisiteChecker& prereqChecker;
};

#endif
