#include "Course.h"
#include "Student.h"
#include <iostream>
#include <sstream>
#include <algorithm>

// Constructor
Course::Course(int id, const std::string& name, const std::string& sched, int cap, const std::string& prereq)
    : courseID(id), courseName(name), schedule(sched), capacity(cap), prereqStr(prereq) {
}

// Destructor
Course::~Course() {}

// Getters
int Course::getCourseID() const { return courseID; }
std::string Course::getCourseName() const { return courseName; }
std::string Course::getSchedule() const { return schedule; }
int Course::getCapacity() const { return capacity; }

const std::vector<Course*>& Course::getPrerequisites() const {
    // If 100-level course, no prerequisites
    if (courseID < 200) {
        static std::vector<Course*> empty;
        return empty;
    }
    return prerequisites;
}

const std::vector<Student*>& Course::getEnrolledStudents() const {
    return enrolledStudents;
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
    if (courseID >= 200 && prerequisite) {
        prerequisites.push_back(prerequisite);
    }
}

// Enrollment Management
bool Course::enrollStudent(Student* student) {
    if (capacity <= 0) {
        std::cout << "Course is full.\n";
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

// Prerequisite Checking
bool Course::checkPrerequisitesMet(Student* student) const {
    // No prerequisites if 100-level course
    if (courseID < 200) {
        return true;
    }

    const auto& regCourses = student->getRegisteredCourses();
    for (Course* prereq : prerequisites) {
        if (std::find(regCourses.begin(), regCourses.end(), prereq) == regCourses.end()) {
            return false;
        }
    }
    return true;
}

void Course::listMissingPrerequisites(Student* student) const {
    std::cout << "Missing prerequisites for course " << courseName << ":\n";
    if (courseID < 200) {
        std::cout << "None required for a 100-level course.\n";
        return;
    }

    const auto& regCourses = student->getRegisteredCourses();
    for (Course* prereq : prerequisites) {
        if (std::find(regCourses.begin(), regCourses.end(), prereq) == regCourses.end()) {
            std::cout << "- " << prereq->getCourseName() << "\n";
        }
    }
}

// Resolve prerequisites by parsing prereqStr
void Course::resolvePrerequisites(const std::vector<Course*>& allCourses) {
    if (courseID < 200 || prereqStr.empty()) return;

    std::istringstream iss(prereqStr);
    std::string token;
    while (std::getline(iss, token, ',')) {
        int prereqID = std::stoi(token);
        for (auto c : allCourses) {
            if (c->getCourseID() == prereqID) {
                addPrerequisite(c);
                break;
            }
        }
    }
}
