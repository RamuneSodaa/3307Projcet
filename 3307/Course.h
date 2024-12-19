#ifndef COURSE_H
#define COURSE_H

#include <string>
#include <vector>

class Student;

class Course {
public:
    Course(int id, const std::string& name, const std::string& sched, int cap, const std::string& prereq = "");
    ~Course();

    int getCourseID() const;
    std::string getCourseName() const;
    std::string getSchedule() const;
    int getCapacity() const;
    const std::vector<Course*>& getPrerequisites() const;

    void setSchedule(const std::string& newSchedule);
    void setCapacity(int newCapacity);
    void setCourseName(const std::string& name);
    void addPrerequisite(Course* prerequisite);

    bool enrollStudent(Student* student);
    void dropStudent(Student* student);

    // Returns the raw prereqStr from DB
    std::string getPrereqStr() const { return prereqStr; }

    // Resolve prerequisites using the given course list
    void resolvePrerequisites(const std::vector<Course*>& allCourses);

private:
    int courseID;
    std::string courseName;
    std::string schedule;
    int capacity;
    std::vector<Course*> prerequisites;
    std::vector<Student*> enrolledStudents;
    std::string prereqStr; // Raw prerequisites string from DB if any

    bool checkPrerequisitesMet(Student* student) const;
    void listMissingPrerequisites(Student* student) const;
};

#endif
