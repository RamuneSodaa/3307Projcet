#include <iostream>
#include "Course.h"
#include "Student.h"

int main() {
    // Create courses
    Course math(101, "Mathematics", "Monday 10:00-12:00", 3);
    Course physics(102, "Physics", "Tuesday 14:00-16:00", 3);
    Course chemistry(103, "Chemistry", "Wednesday 09:00-11:00", 3);

    // Set prerequisites for Chemistry
    chemistry.addPrerequisite(&math);
    chemistry.addPrerequisite(&physics);

    // Add courses to a list
    std::vector<Course*> courseList = {&math, &physics, &chemistry};

    // Print available courses
    std::cout << "Available Courses: \n";
    for (const auto& course : courseList) {
        std::cout << "Course ID: " << course->getCourseID()
                  << ", Name: " << course->getCourseName()
                  << ", Schedule: " << course->getSchedule()
                  << ", Capacity: " << course->getCapacity() << "\n";
    }

    // Create a student
    Student student1(1, "john_doe", "john@example.com", "active");

    // Attempt to enroll student in Chemistry without prerequisites
    std::cout << "\nPrerequisite check failed for course: Chemistry\n";
    chemistry.listMissingPrerequisites(&student1);

    // Enroll the student in Mathematics
    student1.addCourse(&math);
    math.enrollStudent(&student1);

    // Log activity
    student1.logActivity("Added course: Mathematics");

    // Enroll the student in Physics
    student1.addCourse(&physics);
    physics.enrollStudent(&student1);

    // Log activity
    student1.logActivity("Added course: Physics");

    // Attempt to enroll the student in Chemistry after prerequisites are met
    student1.addCourse(&chemistry);
    if (!chemistry.enrollStudent(&student1)) {
        std::cout << "Student cannot enroll in Chemistry due to missing prerequisites.\n";
    } else {
        student1.logActivity("Added course: Chemistry");
    }

    // Print student's schedule
    std::cout << "\nSchedule for Student ID: " << student1.getStudentID() << "\n";
    for (const auto& course : student1.getRegisteredCourses()) {
        std::cout << course->getCourseName() << " - " << course->getSchedule() << "\n";
    }

    return 0;
}
