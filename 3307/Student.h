#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <vector>
#include <ctime>
#include "Course.h"

class Student {
private:
    int studentID; // Unique identifier for the student
    std::string username; // Student's username
    std::string email; // Student's email address
    std::string status; // Student's status (e.g., active, inactive)
    std::vector<Course*> registeredCourses; // List of registered courses
    std::vector<Course*> draftSchedule; // List of courses in the draft schedule
    std::vector<std::string> activityLog; // Log of student activities

    // Helper method to get the current timestamp
    std::string getCurrentTimestamp() const;

public:
    // Constructor and Destructor
    Student(int id, const std::string& uname, const std::string& email, const std::string& status);
    ~Student();
    Student(const std::string& id, const std::string& email);

    // Getters
    int getStudentID() const;
    std::string getUsername() const;
    std::string getEmail() const;
    std::string getStatus() const;
    const std::vector<Course*>& getRegisteredCourses() const;
    std::vector<Course*>& getDraftSchedule();;

    // Setters
    void setStatus(const std::string& newStatus);

    // Course Management
    void addCourse(Course* course); // Add a course to the registered courses
    void addToDraftSchedule(Course* course); // Add a course to the draft schedule
    void dropCourse(int courseID); // Drop a course from registered courses
    void dropFromDraftSchedule(int courseID); // Drop a course from the draft schedule
    void listCourses() const; // List all registered courses
    void clearDraftSchedule();
    void finalizeEnrollment(Course* course);

    // Activity Logging
    void logActivity(const std::string& activityDescription);
    void showActivityLog() const;
};

#endif // STUDENT_H
