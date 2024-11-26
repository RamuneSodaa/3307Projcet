#include <iostream>
#include "Authentication.h"
#include "CourseManager.h"
#include "Scheduler.h"
#include "Student.h"
#include "PrerequisiteChecker.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <filesystem>

int main() {
    // Create a window with SFML
    sf::RenderWindow window(sf::VideoMode(800, 600), "Course Enrollment System");

    // Load a font for SFML (use a relative path)
    // Open source text file from https://fonts.google.com/specimen/Open+Sans
    sf::Font font;
    if (!font.loadFromFile("C:\\Users\\Alexf\\Desktop\\3307Projcet\\alex\\3307\\assets\\fonts\\OpenSans-Regular.ttf")) {
        std::cerr << "Error loading font.\n";
        return -1;
    }

    // Authentication setup
    Authentication* auth = Authentication::getInstance();
    auth->addUser("student1", "password123");

    // User login
    std::string username = "student1";
    std::string password = "password123";
    if (!auth->login(username, password)) {
        std::cerr << "Authentication failed. Exiting...\n";
        return 1;
    }

    // Create Student
    Student student1("S12345", "student1@example.com");

    // Setup CourseManager and Scheduler
    CourseManager courseManager;
    Course* math101 = new Course(101, "Math 101", "Monday 10:00-12:00", 30);
    Course* physics102 = new Course(102, "Physics 102", "Tuesday 14:00-16:00", 25);
    Course* calculus201 = new Course(201, "Calculus 201", "Wednesday 10:00-12:00", 20);

    calculus201->addPrerequisite(math101);
    courseManager.addCourse(math101);
    courseManager.addCourse(physics102);
    courseManager.addCourse(calculus201);

    Scheduler scheduler("Fall 2024");
    scheduler.addCourse(math101);
    scheduler.addCourse(physics102);
    scheduler.addCourse(calculus201);

    // Setup SFML text to display available courses
    sf::Text courseText;
    courseText.setFont(font);
    courseText.setCharacterSize(20);
    courseText.setFillColor(sf::Color::Black);
    courseText.setPosition(50, 50);
    courseText.setString("Available Courses:\n"
                         "1. Math 101 (Monday 10:00-12:00)\n"
                         "2. Physics 102 (Tuesday 14:00-16:00)\n"
                         "3. Calculus 201 (Wednesday 10:00-12:00)");

    // Create "Enroll" button
    sf::RectangleShape enrollButton(sf::Vector2f(150, 50));
    enrollButton.setPosition(50, 300);
    enrollButton.setFillColor(sf::Color::Green);

    sf::Text enrollText;
    enrollText.setFont(font);
    enrollText.setString("Enroll in Math 101");
    enrollText.setCharacterSize(20);
    enrollText.setFillColor(sf::Color::White);
    enrollText.setPosition(60, 310);

    // Main loop for GUI
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Handle button click events
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (enrollButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    // Enroll in Math 101
                    if (scheduler.scheduleCourse(&student1, math101->getCourseID())) {
                        std::cout << "Enrolled in Math 101 successfully!\n";
                    } else {
                        std::cout << "Enrollment in Math 101 failed.\n";
                    }
                }
            }
        }

        // Render GUI elements
        window.clear(sf::Color::White);
        window.draw(courseText);
        window.draw(enrollButton);
        window.draw(enrollText);
        window.display();
    }

    // Clean up
    delete math101;
    delete physics102;
    delete calculus201;

    return 0;
}
