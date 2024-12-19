#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <vector>
#include <ctime>

class Course;

class Student {
public:
    // Existing constructors
    Student(int id, const std::string& uname, const std::string& email, const std::string& status);
    Student(const std::string& id, const std::string& email);

    // Existing destructor
    ~Student();

    // Getters
    int getStudentID() const;
    std::string getUsername() const;
    std::string getEmail() const;
    std::string getStatus() const;
    const std::vector<Course*>& getRegisteredCourses() const;
    std::vector<Course*>& getDraftSchedule();

    // Setters
    void setStatus(const std::string& newStatus);
    void finalizeEnrollment(Course* course);
    void clearDraftSchedule();

    // Add and drop courses
    void addCourse(Course* course);
    void addToDraftSchedule(Course* course);
    void dropCourse(int courseID);
    void dropFromDraftSchedule(int courseID);

    // List courses
    void listCourses() const;

    // Activity log
    void logActivity(const std::string& activityDescription);
    void showActivityLog() const;

    // Additional method: Remove a specific course pointer from registeredCourses
    void removeCourse(Course* course);

private:
    int studentID;
    std::string username;
    std::string email;
    std::string status;
    std::vector<Course*> registeredCourses;
    std::vector<Course*> draftSchedule;
    std::vector<std::string> activityLog;

    std::string getCurrentTimestamp() const;
};

#endif
