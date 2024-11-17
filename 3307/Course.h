#ifndef COURSE_H
#define COURSE_H

#include <string>
#include <vector>

class Student;

class Course {
private:
    int courseID;                           // Unique identifier for the course
    std::string courseName;                 // Name of the course
    std::string schedule;                   // e.g., "Monday 10:00-12:00"
    int capacity;                           // Maximum number of students
    std::vector<Course*> prerequisites;     // List of prerequisite courses
    std::vector<Student*> enrolledStudents; // List of enrolled students
    std::vector<Student*> waitlist;         // Students waiting for a spot

public:
    // Constructor and Destructor
    Course(int id, const std::string& name, const std::string& schedule, int capacity);
    ~Course();

    // Getters
    int getCourseID() const;                      // Get the course ID
    std::string getCourseName() const;            // Get the course name
    std::string getSchedule() const;              // Get the course schedule
    int getCapacity() const;                      // Get the maximum capacity
    const std::vector<Course*>& getPrerequisites() const; // Get prerequisites
    const std::vector<Student*>& getEnrolledStudents() const; // Get enrolled students
    const std::vector<Student*>& getWaitlist() const; // Get the waitlist

    // Setters
    void setSchedule(const std::string& newSchedule); // Update schedule
    void setCapacity(int newCapacity);                // Update capacity
    void setCourseName(const std::string& name);      // Update course name
    void addPrerequisite(Course* prerequisite);       // Add a prerequisite course

    // Enrollment and Waitlist Management
    bool enrollStudent(Student* student); // Enroll a student
    void dropStudent(Student* student);   // Drop a student
    void addToWaitlist(Student* student); // Add a student to the waitlist
    void removeFromWaitlist(Student* student); // Remove a student from the waitlist

    // Status Checkers
    bool isFull() const;               // Check if course is full
    bool hasPrerequisite(Course* course) const; // Check if a specific prerequisite is present
    bool checkPrerequisitesMet(Student* student) const; // Check if student meets prerequisites

    // Notifications
    void notifyWaitlist();             // Notify students in the waitlist

    // Utility for prerequisites
    void listMissingPrerequisites(Student* student) const; // List missing prerequisites for a student
};

#endif // COURSE_H
