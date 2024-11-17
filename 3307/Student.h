#ifndef STUDENT_H
#define STUDENT_H

#include "Course.h"
#include <vector>
#include <string>
#include <ctime>

class Student {
private:
    int studentID;
    std::string username;
    std::string email;
    std::string status;
    std::vector<Course*> registeredCourses;
    std::vector<std::string> activityLog; // Keeps a log of student activities

    // Helper function to get the current timestamp for logs
    std::string getCurrentTimestamp() const;

public:
    // Constructor and Destructor
    Student(int id, const std::string& uname, const std::string& email, const std::string& status);
    ~Student();

    // Getters
    int getStudentID() const;
    std::string getUsername() const;
    std::string getEmail() const;
    std::string getStatus() const;
    const std::vector<Course*>& getRegisteredCourses() const;

    // Setters
    void setStatus(const std::string& newStatus);

    // Methods for course management
    void addCourse(Course* course);
    void dropCourse(int courseID);
    void listCourses() const;

    // Logging
    void logActivity(const std::string& activityDescription);
    void showActivityLog() const;
};

#endif // STUDENT_H
