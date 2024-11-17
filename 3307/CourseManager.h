#ifndef COURSEMANAGER_H
#define COURSEMANAGER_H

#include <vector>
#include "Course.h"

class CourseManager {
private:
    std::vector<Course*> courseList; // Stores all available courses

public:
    // Constructor and Destructor
    CourseManager();
    ~CourseManager();

    // Course management methods
    void addCourse(Course* course);
    bool removeCourse(int courseID);
    Course* searchCourse(int courseID) const;

    // Utility methods
    void browseCourses() const;
    void displayCourseDetails(int courseID) const;

    // Advanced feature
    std::vector<Course*> filterCoursesByDepartment(const std::string& department) const;
};

#endif // COURSEMANAGER_H
