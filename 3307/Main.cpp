#include <iostream>
#include "Authentication.h"
#include "CourseManager.h"
#include "Scheduler.h"
#include "Student.h"
#include "Course.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <algorithm>  // To use std::remove_if for trimming

// Function to trim whitespace from both ends of a string
std::string trim(const std::string& str) {
    auto start = str.begin();
    while (start != str.end() && std::isspace(*start)) {
        start++;
    }

    auto end = str.end();
    do {
        end--;
    } while (std::distance(start, end) > 0 && std::isspace(*end));

    return std::string(start, end + 1);
}

int main() {
    // Create a window with SFML
    sf::RenderWindow window(sf::VideoMode(800, 600), "Course Enrollment System");

    // Load a font for SFML (use a relative path)
    sf::Font font;
    if (!font.loadFromFile("C:\\Users\\Alexf\\Desktop\\3307Projcet\\alex\\3307\\assets\\fonts\\OpenSans-Regular.ttf")) {
        std::cerr << "Error loading font.\n";
        return -1;
    }

    // Authentication setup
    Authentication* auth = Authentication::getInstance();
    auth->addUser("student1", "password123");

    // Text fields for user input
    sf::Text usernameLabel("Username: ", font, 20);
    usernameLabel.setPosition(50, 50);
    usernameLabel.setFillColor(sf::Color::Black);

    sf::Text passwordLabel("Password: ", font, 20);
    passwordLabel.setPosition(50, 100);
    passwordLabel.setFillColor(sf::Color::Black);

    sf::Text usernameInput("", font, 20);
    usernameInput.setPosition(200, 50);
    usernameInput.setFillColor(sf::Color::Blue);

    sf::Text passwordInput("", font, 20);
    passwordInput.setPosition(200, 100);
    passwordInput.setFillColor(sf::Color::Blue);

    std::string username = "";
    std::string password = "";
    bool enteringUsername = true;  // Track which input field is active

    // Login button
    sf::RectangleShape loginButton(sf::Vector2f(100, 40));
    loginButton.setPosition(50, 150);
    loginButton.setFillColor(sf::Color::Green);

    sf::Text loginButtonText("Login", font, 20);
    loginButtonText.setFillColor(sf::Color::White);
    loginButtonText.setPosition(60, 160);

    // CourseManager and Student Setup
    Student student1("S12345", "student1@example.com");
    CourseManager courseManager;
    Course* math101 = new Course(101, "Math 101", "Monday 10:00-12:00", 30);
    Course* physics102 = new Course(102, "Physics 102", "Tuesday 14:00-16:00", 25);
    courseManager.addCourse(math101);
    courseManager.addCourse(physics102);

    bool isLoggedIn = false;

    // Main loop for GUI
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Handle text entry for login fields
            if (event.type == sf::Event::TextEntered) {
                if (enteringUsername) {
                    if (event.text.unicode == '\b' && !username.empty()) {
                        username.pop_back();  // Handle backspace
                    } else if (event.text.unicode < 128 && event.text.unicode != '\b') {
                        username += static_cast<char>(event.text.unicode);
                    }
                    usernameInput.setString(username);
                } else {
                    if (event.text.unicode == '\b' && !password.empty()) {
                        password.pop_back();  // Handle backspace
                    } else if (event.text.unicode < 128 && event.text.unicode != '\b') {
                        password += static_cast<char>(event.text.unicode);
                    }
                    passwordInput.setString(password);
                }
            }

            // Handle Enter or Tab key to switch between username and password, or to login
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Tab || event.key.code == sf::Keyboard::Enter) {
                    if (enteringUsername) {
                        enteringUsername = false;  // Switch to password input
                    } else if (!enteringUsername && !username.empty() && !password.empty()) {
                        // If Enter is pressed and both fields are filled, try to log in
                        std::string trimmedUsername = trim(username);
                        std::string trimmedPassword = trim(password);
                        std::cout << "Trying to log in with Username: '" << trimmedUsername << "' and Password: '" << trimmedPassword << "'\n";

                        if (auth->login(trimmedUsername, trimmedPassword)) {
                            std::cout << "Login successful!\n";
                            isLoggedIn = true;

                            // Automatically enroll the student in Math 101 for demonstration purposes
                            bool enrolled = math101->enrollStudent(&student1);
                            if (enrolled) {
                                std::cout << "Successfully enrolled in " << math101->getCourseName() << "\n";
                            } else {
                                std::cout << "Failed to enroll in " << math101->getCourseName() << ".\n";
                            }
                        } else {
                            std::cerr << "Authentication failed. Please try again.\n";
                        }
                    }
                }
            }

            // Handle button click events
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                std::cout << "Mouse click position: " << mousePos.x << ", " << mousePos.y << std::endl;

                // Handle login button click
                if (loginButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    std::string trimmedUsername = trim(username);
                    std::string trimmedPassword = trim(password);
                    std::cout << "Login button clicked with Username: '" << trimmedUsername << "' and Password: '" << trimmedPassword << "'\n";

                    if (auth->login(trimmedUsername, trimmedPassword)) {
                        std::cout << "Login successful!\n";
                        isLoggedIn = true;

                        // Automatically enroll the student in Math 101 for demonstration purposes
                        bool enrolled = math101->enrollStudent(&student1);
                        if (enrolled) {
                            std::cout << "Successfully enrolled in " << math101->getCourseName() << "\n";
                        } else {
                            std::cout << "Failed to enroll in " << math101->getCourseName() << ".\n";
                        }
                    } else {
                        std::cerr << "Authentication failed. Please try again.\n";
                    }
                }
            }
        }

        // Render GUI elements
        window.clear(sf::Color::White);

        if (!isLoggedIn) {
            // Draw login fields
            window.draw(usernameLabel);
            window.draw(passwordLabel);
            window.draw(usernameInput);
            window.draw(passwordInput);
            window.draw(loginButton);
            window.draw(loginButtonText);
        } else {
            // Display enrolled courses
            sf::Text enrolledCoursesText("Enrolled Courses:", font, 20);
            enrolledCoursesText.setPosition(50, 50);
            enrolledCoursesText.setFillColor(sf::Color::Black);
            window.draw(enrolledCoursesText);

            // If the student is enrolled, show the courses
            int yPosition = 80;
            for (Student* student : math101->getEnrolledStudents()) {
                if (student->getStudentID() == student1.getStudentID()) {
                    sf::Text courseText(math101->getCourseName() + " (" + math101->getSchedule() + ")", font, 20);
                    courseText.setPosition(50, yPosition);
                    courseText.setFillColor(sf::Color::Black);
                    window.draw(courseText);
                    yPosition += 30;
                }
            }
        }

        window.display();
    }

    // Clean up
    delete math101;
    delete physics102;

    return 0;
}
