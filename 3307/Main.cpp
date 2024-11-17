#include "Scheduler.h"
#include "Student.h"
#include "Course.h"

int main() {
    // Create courses
    Course math(101, "Mathematics", "Monday 10:00-12:00", 3);
    Course physics(102, "Physics", "Tuesday 14:00-16:00", 3);
    Course chemistry(103, "Chemistry", "Wednesday 09:00-11:00", 3);

    // Add prerequisites
    chemistry.addPrerequisite(&math);
    chemistry.addPrerequisite(&physics);

    // Create a student
    Student student1(1, "john_doe", "john.doe@example.com", "active");

    // Create a scheduler
    Scheduler scheduler("Fall 2024");

    // Add course offerings
    scheduler.addCourse(&math);
    scheduler.addCourse(&physics);
    scheduler.addCourse(&chemistry);

    // Display course offerings
    scheduler.displayCourseOfferings();

    // Attempt to enroll in a course without prerequisites
    scheduler.scheduleCourse(&student1, 103); // Should fail

    // Enroll in prerequisites
    scheduler.scheduleCourse(&student1, 101); // Math
    scheduler.scheduleCourse(&student1, 102); // Physics

    // Attempt to enroll again
    scheduler.scheduleCourse(&student1, 103); // Should succeed

    // Generate the student's schedule
    scheduler.generateSchedule(&student1);

    return 0;
}
