#ifndef COURSE_MANAGER_H
#define COURSE_MANAGER_H

#include <vector>
#include <string>
#include "Course.h"

class CourseManager {
public:
    CourseManager();
    ~CourseManager();

    void addCourse(Course* course);
    bool removeCourse(int courseID);

    // New unified search method:
    // If query is numeric, search by ID.
    // If query is non-numeric, search by partial name.
    std::vector<Course*> searchCourses(const std::string& query) const;

    void browseCourses() const;
    void displayCourseDetails(int courseID) const;

    std::vector<Course*> filterCoursesByDepartment(const std::string& department) const;
    const std::vector<Course*>& getCourseList() const;

private:
    std::vector<Course*> courseList;
};

#endif // COURSE_MANAGER_H
