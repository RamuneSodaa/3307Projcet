#include "EnrollmentManager.h"
#include "CourseManager.h"
#include "Scheduler.h"
#include "PrerequisiteChecker.h"
#include "Student.h"
#include "Course.h"
#include "DatabaseManager.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include "NotificationInterface.h"

/**
 * Constructs the EnrollmentManager with the specified managers and checker.
 *
 * @param cm Reference to the CourseManager instance.
 * @param sched Reference to the Scheduler instance.
 * @param checker Reference to the PrerequisiteChecker instance.
 */
EnrollmentManager::EnrollmentManager(CourseManager& cm, Scheduler& sched, PrerequisiteChecker& checker)
: courseManager(cm), scheduler(sched), prereqChecker(checker) {
}

/**
 * Enrolls a student in a specified course by ID if all conditions are met.
 *
 * @param student Pointer to the Student object.
 * @param courseID ID of the course to enroll in.
 * @return True if enrollment is successful, false otherwise.
 */
bool EnrollmentManager::enrollStudentInCourse(Student* student, int courseID) {
    if (!student) {
        NotificationInterface::show("\nError: Student object is null.\n");
        return false;
    }

    // Check if already enrolled in this course
    for (auto enrolledCourse : student->getRegisteredCourses()) {
        if (enrolledCourse->getCourseID() == courseID) {
            NotificationInterface::show("\nStudent is already enrolled in course ID: " + courseID);
            return false;
        }
    }

    auto results = courseManager.searchCourses(std::to_string(courseID));
    if (results.empty()) {
        NotificationInterface::show("\nCourse not found with ID: " + courseID);
        return false;
    }

    Course* course = results.front();

    // Check prerequisites
    if (!prereqChecker.verifyPrerequisites(student, course)) {
        NotificationInterface::show("\nPrerequisite check failed for course: " + course->getCourseName());
        prereqChecker.displayMissingPrerequisites(student, course);
        return false;
    }

    // Attempt to schedule using Scheduler
    if (!scheduler.scheduleCourse(student, courseID)) {
        NotificationInterface::show("\nFailed to enroll in course ID: " + courseID);
        return false;
    }

    // If here, scheduling successful. Now record in DB
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);
    std::ostringstream oss;
    oss << (now->tm_year + 1900) << "-"
        << std::setw(2) << std::setfill('0') << (now->tm_mon + 1) << "-"
        << std::setw(2) << std::setfill('0') << now->tm_mday;

    std::string enrollmentDate = oss.str();
    std::string courseName = course->getCourseName(); // Retrieve the course name

    if (!addEnrollment(student->getStudentID(), courseID, courseName, enrollmentDate)) {
        std::cout << "Failed to add enrollment record in the database.\n";
        return false;
    }

    NotificationInterface::show("Enrollment successful for student: " + student->getUsername() + " in course: " + course->getCourseName()  ) ;
    return true;
}

/**
 * Adds an enrollment record to the database.
 *
 * @param studentID ID of the student.
 * @param courseID ID of the course.
 * @param courseName Name of the course.
 * @param enrollmentDate Date of enrollment.
 * @return True if the record was added successfully, false otherwise.
 */
bool EnrollmentManager::addEnrollment(int studentID, int courseID, const std::string& courseName, const std::string& enrollmentDate) {
    DatabaseManager::addEnrollment(studentID, courseID, courseName, enrollmentDate);
    std::cout << "Enrollment record added: StudentID=" << studentID
              << ", CourseID=" << courseID
              << ", CourseName=" << courseName
              << ", Date=" << enrollmentDate << "\n";
    return true;
}

/**
 * Drops a student's enrollment from a specific course.
 *
 * @param student Pointer to the Student object.
 * @param courseID ID of the course from which to unenroll the student.
 * @return True if the enrollment was successfully removed, false otherwise.
 */
bool EnrollmentManager::dropEnrollment(Student* student, int courseID) {
    if (!student) {
        std::cerr << "Error: Student is null.\n";
        return false;
    }

    // Find the course in student's registered courses
    auto& registered = student->getRegisteredCourses();
    Course* targetCourse = nullptr;
    for (auto c : registered) {
        if (c->getCourseID() == courseID) {
            targetCourse = c;
            break;
        }
    }

    if (!targetCourse) {
        std::cout << "Student is not enrolled in course ID: " << courseID << "\n";
        return false;
    }

    // Remove the enrollment from the database
    if (!DatabaseManager::removeEnrollment(student->getStudentID(), courseID)) {
        std::cout << "Failed to remove enrollment from database for course ID: " << courseID << "\n";
        return false;
    }

    // Drop the student from the course object
    targetCourse->dropStudent(student);

    // Remove the course from student's registered list
    student->removeCourse(targetCourse);

    std::cout << "Enrollment removed for student: " << student->getUsername()
              << " from course: " << targetCourse->getCourseName() << "\n";

    return true;
}

