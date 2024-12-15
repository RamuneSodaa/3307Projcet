#include "CourseManager.h"
#include <iostream>
#include <algorithm>

// Constructor
CourseManager::CourseManager() {
    // Initialize the course list with some default courses
    courseList.push_back(new Course(101, "Mathematics", "Mon-Wed", 30));
    courseList.push_back(new Course(102, "Physics", "Tue-Thu", 25));
    courseList.push_back(new Course(103, "Chemistry", "Mon-Fri", 20));

    std::cout << "CourseManager initialized with default courses.\n";
}

// Destructor
CourseManager::~CourseManager() {
    for (auto& course : courseList) {
        delete course; // Clean up dynamically allocated courses
    }
}

// Add a course to the system
void CourseManager::addCourse(Course* course) {
    courseList.push_back(course);
    std::cout << "Course added: " << course->getCourseName() << " (ID: " << course->getCourseID() << ")" << std::endl;
}

// Remove a course from the system by ID
bool CourseManager::removeCourse(int courseID) {
    auto it = std::remove_if(courseList.begin(), courseList.end(),
        [courseID](Course* course) { return course->getCourseID() == courseID; });

    if (it != courseList.end()) {
        delete *it; // Free the memory of the removed course
        courseList.erase(it, courseList.end());
        std::cout << "Course with ID " << courseID << " removed." << std::endl;
        return true;
    }
    std::cout << "Course with ID " << courseID << " not found." << std::endl;
    return false;
}

// Search for a course by ID
Course* CourseManager::searchCourse(int courseID) const {
    for (auto& course : courseList) {
        if (course->getCourseID() == courseID) {
            return course;
        }
    }
    return nullptr; // Course not found
}

// Display all available courses
void CourseManager::browseCourses() const {
    std::cout << "Available Courses:" << std::endl;
    for (const auto& course : courseList) {
        std::cout << "ID: " << course->getCourseID()
                  << ", Name: " << course->getCourseName()
                  << ", Schedule: " << course->getSchedule()
                  << ", Capacity: " << course->getCapacity() << std::endl;
    }
}

// Display details for a specific course by ID
void CourseManager::displayCourseDetails(int courseID) const {
    Course* course = searchCourse(courseID);
    if (course) {
        std::cout << "Course Details:" << std::endl;
        std::cout << "ID: " << course->getCourseID() << std::endl;
        std::cout << "Name: " << course->getCourseName() << std::endl;
        std::cout << "Schedule: " << course->getSchedule() << std::endl;
        std::cout << "Capacity: " << course->getCapacity() << std::endl;
        std::cout << "Prerequisites: ";
        for (auto& prereq : course->getPrerequisites()) {
            std::cout << prereq->getCourseName() << " ";
        }
        std::cout << std::endl;
    } else {
        std::cout << "No details available for Course ID: " << courseID << std::endl;
    }
}

// Filter courses by department (optional feature for future)
std::vector<Course*> CourseManager::filterCoursesByDepartment(const std::string& department) const {
    std::vector<Course*> filteredCourses;
    for (const auto& course : courseList) {
        if (course->getCourseName().find(department) != std::string::npos) { // Example filter logic
            filteredCourses.push_back(course);
        }
    }
    return filteredCourses;
}

// Expose the course list
const std::vector<Course*>& CourseManager::getCourseList() const {
    return courseList;
}

void CourseManager::initializeCourses() {
    courseList.push_back(new Course(101, "Mathematics", "Mon-Wed", 30));
    courseList.push_back(new Course(102, "Physics", "Tue-Thu", 25));
    courseList.push_back(new Course(103, "Chemistry", "Mon-Fri", 20));

    std::cout << "Courses initialized.\n";
}

// Implement browseCoursesUI
void CourseManager::browseCoursesUI(sf::RenderWindow& window, sf::Font& font) const {
    sf::RenderWindow browseWindow(sf::VideoMode(800, 600), "Browse Courses");
    std::vector<Course*> courses = getCourseList();
    std::vector<sf::Text> courseTexts;

    // Prepare text for each course
    int yPosition = 50;
    for (const auto& course : courses) {
        sf::Text courseText;
        courseText.setFont(font);
        courseText.setString("ID: " + std::to_string(course->getCourseID()) + " Name: " + course->getCourseName());
        courseText.setCharacterSize(20);
        courseText.setPosition(50, yPosition);
        courseText.setFillColor(sf::Color::Black);
        courseTexts.push_back(courseText);
        yPosition += 30; // Adjust spacing
    }

    // Scroll controls
    int scrollOffset = 0;

    // Main loop
    while (browseWindow.isOpen()) {
        sf::Event event;
        while (browseWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                browseWindow.close();
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    scrollOffset = std::max(0, scrollOffset - 20); // Scroll up
                } else if (event.key.code == sf::Keyboard::Down) {
                    scrollOffset += 20; // Scroll down
                }
            }
        }

        browseWindow.clear(sf::Color::White);

        // Draw course texts
        for (auto& courseText : courseTexts) {
            courseText.move(0, -scrollOffset);
            browseWindow.draw(courseText);
            courseText.move(0, scrollOffset); // Reset position for future draws
        }

        browseWindow.display();
    }
}
