#ifndef COURSEMANAGER_H
#define COURSEMANAGER_H

#include <vector>
#include <string>

class Course;

class CourseManager {
public:
    CourseManager();
    ~CourseManager();

    void loadCoursesFromDatabase(); // Now public

    void addCourse(Course* course);
    bool removeCourse(int courseID);
    std::vector<Course*> searchCourses(const std::string& query) const;
    void browseCourses() const;
    void displayCourseDetails(int courseID) const;
    const std::vector<Course*>& getCourseList() const;

private:
    std::vector<Course*> courseList;
};

#endif
