#include "Course.h"
#include "Student.h"
#include <iostream>
#include <sstream>
#include <algorithm>

Course::Course(int id, const std::string& name, const std::string& sched, int cap, const std::string& prereq)
    : courseID(id), courseName(name), schedule(sched), capacity(cap), prereqStr(prereq) {
}

Course::~Course() { }

int Course::getCourseID() const { return courseID; }
std::string Course::getCourseName() const { return courseName; }
std::string Course::getSchedule() const { return schedule; }
int Course::getCapacity() const { return capacity; }

const std::vector<Course*>& Course::getPrerequisites() const {
    return prerequisites;
}

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
    if (prerequisite) {
        prerequisites.push_back(prerequisite);
    }
}

bool Course::enrollStudent(Student* student) {
    if (capacity <= 0) {
        std::cout << "Course is full.\n";
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
        capacity++; // free up a slot
    }
}

void Course::resolvePrerequisites(const std::vector<Course*>& allCourses) {
    if (courseID < 200 || prereqStr.empty()) return; // no prereqs for <200, or empty string

    std::istringstream iss(prereqStr);
    std::string token;
    while (std::getline(iss, token, ',')) {
        // Trim spaces
        token.erase(token.begin(), std::find_if(token.begin(), token.end(), [](unsigned char ch){ return !std::isspace(ch); }));
        token.erase(std::find_if(token.rbegin(), token.rend(), [](unsigned char ch){ return !std::isspace(ch); }).base(), token.end());

        if (token.empty()) {
            // If empty token, just continue
            continue;
        }

        // Check if token is numeric
        if (!std::all_of(token.begin(), token.end(), ::isdigit)) {
            std::cerr << "Prerequisite token '" << token << "' is not numeric. Skipping.\n";
            continue; // skip non-numeric token
        }

        int prereqID;
        try {
            prereqID = std::stoi(token);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid prerequisite token '" << token << "', cannot convert to int. Skipping.\n";
            continue; // skip invalid token
        }

        // Now find the course with prereqID
        bool found = false;
        for (auto c : allCourses) {
            if (c->getCourseID() == prereqID) {
                addPrerequisite(c);
                found = true;
                break;
            }
        }
        if (!found) {
            std::cerr << "No course found with ID " << prereqID << " for prerequisite of course ID " << courseID << ".\n";
        }
    }
}


// The following methods (checkPrerequisitesMet, listMissingPrerequisites) may be defined if needed,
// but PrerequisiteChecker uses its own logic so we can omit them here or keep empty.
