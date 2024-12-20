#include "Scheduler.h"
#include "Course.h"
#include "Student.h"
#include <iostream>
#include <algorithm>

/**
 * Constructs a Scheduler object with a specific semester.
 *
 * @param semester The name of the semester for scheduling.
 */
Scheduler::Scheduler(const std::string& semester) : semester(semester) {}

// Destructor
Scheduler::~Scheduler() {
    for (auto course : courseOfferings) {
        delete course;
    }
}

/**
 * Adds a course to the scheduler if the course is not null.
 *
 * @param course Pointer to the course to be added.
 */
void Scheduler::addCourse(Course* course) {
    if (!course) {
        std::cerr << "Cannot add a null course.\n";
        return;
    }
    courseOfferings.push_back(course);
    std::cout << "Course added: " << course->getCourseName() << "\n";
}

/**
 * Removes a course from the scheduler based on its ID.
 *
 * @param courseID The ID of the course to remove.
 */
void Scheduler::removeCourse(int courseID) {
    auto it = std::remove_if(courseOfferings.begin(), courseOfferings.end(),
                             [courseID](Course* course) { return course->getCourseID() == courseID; });
    if (it != courseOfferings.end()) {
        delete *it;
        courseOfferings.erase(it);
        std::cout << "Course removed: " << courseID << "\n";
    } else {
        std::cerr << "Course not found with ID: " << courseID << "\n";
    }
}

/**
 * Updates the schedule of a course identified by its ID.
 *
 * @param courseID The ID of the course to update.
 * @param schedule The new schedule string.
 */
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

/**
 * Attempts to enroll a student in a course by its ID.
 *
 * @param student Pointer to the student to be enrolled.
 * @param courseID The ID of the course to enroll the student in.
 * @return True if enrollment is successful, false otherwise.
 */
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
                handleEnrollmentFailure(student, course);
                return false;
            }
        }
    }
    std::cerr << "Course not found with ID: " << courseID << "\n";
    return false;
}

/**
 * Enrolls a student from their draft schedule.
 *
 * @param student Pointer to the student to be enrolled.
 * @return True if all enrollments are successful, false otherwise.
 */
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

/**
 * Handles the failure of enrollment for a student in a course.
 *
 * @param student Pointer to the student whose enrollment failed.
 * @param course Pointer to the course in which enrollment was attempted.
 */
void Scheduler::handleEnrollmentFailure(Student* student, Course* course) {
    // No waitlist or isFull references now. Just print a generic error.
    std::cout << "Enrollment failed for " << course->getCourseName()
              << ": prerequisites not met or capacity reached.\n";
}

/**
 * Optimizes a schedule for a student based on desired course IDs.
 *
 * @param desiredCourses Vector of desired course IDs.
 * @return Vector of pointers to the courses that were successfully optimized.
 */
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

/**
 * Validates a student's draft schedule to check for any prerequisites not met.
 *
 * @param student Pointer to the student whose draft schedule is being validated.
 * @return Vector of strings containing errors found during validation.
 */
std::vector<std::string> Scheduler::validateDraftSchedule(Student* student) {
    std::vector<std::string> errors;

    auto draftSchedule = student->getDraftSchedule();
    for (auto course : draftSchedule) {
        if (!prerequisiteChecker.verifyPrerequisites(student, course)) {
            errors.push_back("Prerequisites not met for " + course->getCourseName());
        }
        // No isFull check since we removed that logic
    }

    return errors;
}

/**
 * Finalizes a student's draft schedule, enrolling them in all courses if no errors are found.
 *
 * @param student Pointer to the student whose draft schedule is being finalized.
 * @return True if the draft schedule is successfully finalized, false if there are errors.
 */
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
