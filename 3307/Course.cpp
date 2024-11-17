#include "Course.h"
#include "Student.h"
#include <iostream>

// Constructor
Course::Course(int id, const std::string& name, const std::string& schedule, int capacity)
    : courseID(id), courseName(name), schedule(schedule), capacity(capacity) {}

// Destructor
Course::~Course() {}

// Getters
int Course::getCourseID() const {
    return courseID;
}

std::string Course::getCourseName() const {
    return courseName;
}

std::string Course::getSchedule() const {
    return schedule;
}

int Course::getCapacity() const {
    return capacity;
}

const std::vector<Course*>& Course::getPrerequisites() const {
    return prerequisites;
}

const std::vector<Student*>& Course::getEnrolledStudents() const {
    return enrolledStudents;
}

const std::vector<Student*>& Course::getWaitlist() const {
    return waitlist;
}

// Setters
void Course::setSchedule(const std::string& newSchedule) {
    schedule = newSchedule;
}

void Course::setCapacity(int newCapacity) {
    capacity = newCapacity;
}

void Course::setCourseName(const std::string& name) {
    courseName = name;
}

void Course::addPrerequisite(Course* prerequisite) {
    prerequisites.push_back(prerequisite);
}

// Enrollment and Waitlist Management
bool Course::enrollStudent(Student* student) {
    if (isFull()) {
        addToWaitlist(student);
        return false;
    }
    if (!checkPrerequisitesMet(student)) {
        listMissingPrerequisites(student);
        return false;
    }
    enrolledStudents.push_back(student);
    capacity--;
    return true;
}

void Course::dropStudent(Student* student) {
    auto it = std::find(enrolledStudents.begin(), enrolledStudents.end(), student);
    if (it != enrolledStudents.end()) {
        enrolledStudents.erase(it);
        capacity++;
    }
}

void Course::addToWaitlist(Student* student) {
    waitlist.push_back(student);
}

void Course::removeFromWaitlist(Student* student) {
    auto it = std::find(waitlist.begin(), waitlist.end(), student);
    if (it != waitlist.end()) {
        waitlist.erase(it);
    }
}

// Status Checkers
bool Course::isFull() const {
    return capacity <= 0;
}

bool Course::hasPrerequisite(Course* course) const {
    return std::find(prerequisites.begin(), prerequisites.end(), course) != prerequisites.end();
}

bool Course::checkPrerequisitesMet(Student* student) const {
    for (Course* prereq : prerequisites) {
        if (std::find(student->getRegisteredCourses().begin(), student->getRegisteredCourses().end(), prereq) == student->getRegisteredCourses().end()) {
            return false;
        }
    }
    return true;
}

void Course::listMissingPrerequisites(Student* student) const {
    std::cout << "Missing prerequisites for course " << courseName << ":\n";
    for (Course* prereq : prerequisites) {
        if (std::find(student->getRegisteredCourses().begin(), student->getRegisteredCourses().end(), prereq) == student->getRegisteredCourses().end()) {
            std::cout << "- " << prereq->getCourseName() << "\n";
        }
    }
}

// Notifications
void Course::notifyWaitlist() {
    for (Student* student : waitlist) {
        std::cout << "Notifying student: " << student->getUsername() << "\n";
    }
}
