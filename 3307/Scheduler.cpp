#include "Scheduler.h"
#include <iostream>
#include <algorithm>

// Constructor
Scheduler::Scheduler(const std::string& semester) : semester(semester) {}

// Destructor
Scheduler::~Scheduler() {
    for (auto course : courseOfferings) {
        delete course; // Ensure proper cleanup
    }
}

// Getters
std::vector<Course*> Scheduler::getCourseOfferings() const {
    return courseOfferings;
}

// Add a course to the offerings
void Scheduler::addCourse(Course* course) {
    if (!course) {
        std::cerr << "Cannot add a null course.\n";
        return;
    }
    courseOfferings.push_back(course);
    std::cout << "Course added: " << course->getCourseName() << "\n";
}

// Remove a course by ID
void Scheduler::removeCourse(int courseID) {
    auto it = std::remove_if(courseOfferings.begin(), courseOfferings.end(),
                             [courseID](Course* course) { return course->getCourseID() == courseID; });
    if (it != courseOfferings.end()) {
        delete *it; // Free memory
        courseOfferings.erase(it);
        std::cout << "Course removed: " << courseID << "\n";
    } else {
        std::cerr << "Course not found with ID: " << courseID << "\n";
    }
}

// Update course details
void Scheduler::updateCourseDetails(int courseID, const std::string& schedule) {
    for (auto course : courseOfferings) {
        if (course->getCourseID() == courseID) {
            course->setSchedule(schedule);
            std::cout << "Course ID " << courseID << " updated with new schedule: " << schedule << "\n";
            return;
        }
    }
    std::cerr << "Course not found with ID: " << courseID << "\n";
}

// Schedule a course for a student
bool Scheduler::scheduleCourse(Student* student, int courseID) {
    if (!student) {
        std::cerr << "Student object cannot be null.\n";
        return false;
    }

    for (auto course : courseOfferings) {
        if (course->getCourseID() == courseID) {
            // Check prerequisites
            if (!prerequisiteChecker.verifyPrerequisites(student, course)) {
                std::cout << "Prerequisite check failed for course: " << course->getCourseName() << "\n";
                prerequisiteChecker.displayMissingPrerequisites(student, course);
                return false;
            }

            // Enroll the student
            if (course->enrollStudent(student)) {
                student->addCourse(course);
                std::cout << "Student enrolled in course: " << course->getCourseName() << "\n";
                return true;
            } else {
                handleEnrollmentFailure(student, course); // NEW: Handle enrollment failure
                return false;
            }
        }
    }
    std::cerr << "Course not found with ID: " << courseID << "\n";
    return false;
}

// Generate the student's schedule
void Scheduler::generateSchedule(Student* student) const {
    if (!student) {
        std::cerr << "Student object cannot be null.\n";
        return;
    }

    std::cout << "Schedule for Student ID: " << student->getStudentID() << "\n";
    for (const auto& course : student->getRegisteredCourses()) {
        std::cout << course->getCourseName() << " - " << course->getSchedule() << "\n";
    }
}

// Enroll from draft schedule
bool Scheduler::enrollFromDraft(Student* student) {
    for (auto course : student->getDraftSchedule()) {
        if (!scheduleCourse(student, course->getCourseID())) {
            std::cout << "Enrollment failed for course: " << course->getCourseName() << "\n";
            return false;
        }
    }
    std::cout << "All courses from the draft schedule have been enrolled.\n";
    return true;
}

// Drop a course from draft schedule
void Scheduler::dropFromDraft(Student* student, int courseID) {
    auto& draftSchedule = student->getDraftSchedule();
    draftSchedule.erase(
        std::remove_if(
            draftSchedule.begin(),
            draftSchedule.end(),
            [courseID](Course* course) {
                return course->getCourseID() == courseID;
            }
        ),
        draftSchedule.end()
    );
    std::cout << "Course with ID " << courseID << " removed from draft schedule.\n";
}


// Handle enrollment failures
void Scheduler::handleEnrollmentFailure(Student* student, Course* course) {
    if (course->isFull()) {
        course->addToWaitlist(student);
        std::cout << "Course is full. Student added to waitlist for " << course->getCourseName() << "\n";
    } else {
        std::cout << "Student cannot enroll due to unmet prerequisites.\n";
    }
}

// Optimize the student's schedule
std::vector<Course*> Scheduler::optimizeSchedule(const std::vector<int>& desiredCourses) {
    std::vector<Course*> optimizedCourses;
    for (int courseID : desiredCourses) {
        for (const auto& course : courseOfferings) {
            if (course->getCourseID() == courseID) {
                optimizedCourses.push_back(course);
                break;
            }
        }
    }
    return optimizedCourses;
}

// Display all available courses
void Scheduler::displayCourseOfferings() const {
    if (courseOfferings.empty()) {
        std::cout << "No courses available.\n";
        return;
    }

    std::cout << "Available Courses:\n";
    for (const auto& course : courseOfferings) {
        std::cout << "Course ID: " << course->getCourseID()
                  << ", Name: " << course->getCourseName()
                  << ", Schedule: " << course->getSchedule()
                  << ", Capacity: " << course->getCapacity() << "\n";
    }
}
// Validate draft schedule for prerequisites and availability
std::vector<std::string> Scheduler::validateDraftSchedule(Student* student) {
    std::vector<std::string> errors;

    auto draftSchedule = student->getDraftSchedule();
    for (auto course : draftSchedule) {
        if (!prerequisiteChecker.verifyPrerequisites(student, course)) {
            errors.push_back("Prerequisites not met for " + course->getCourseName());
        } else if (course->isFull()) {
            errors.push_back("Course is full: " + course->getCourseName());
        }
    }

    return errors;
}

// Finalize enrollment from draft schedule
bool Scheduler::finalizeDraftSchedule(Student* student) {
    auto errors = validateDraftSchedule(student);

    if (!errors.empty()) {
        std::cout << "Cannot finalize draft schedule. Errors:\n";
        for (const auto& error : errors) {
            std::cout << "- " << error << "\n";
        }
        return false;
    }

    for (auto course : student->getDraftSchedule()) {
        course->enrollStudent(student);
        student->finalizeEnrollment(course);
    }

    std::cout << "Draft schedule successfully finalized.\n";
    return true;
}

