#include "MainInterface.h"
#include "Authentication.h"
#include "CourseManager.h"
#include "Scheduler.h"
#include "Student.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <algorithm> // for std::max
#include <cctype>

// External variables (defined in main.cpp)
extern Student* currentStudent;
extern Authentication* auth;
extern CourseManager courseManager;
extern Scheduler scheduler;

// Utility function to trim whitespace
static std::string trim(const std::string& str) {
    auto start = str.begin();
    while (start != str.end() && std::isspace((unsigned char)*start)) {
        start++;
    }

    auto end = str.end();
    do {
        end--;
    } while (std::distance(start, end) > 0 && std::isspace((unsigned char)*end));

    return std::string(start, end + 1);
}

bool showLoginPage() {
    sf::RenderWindow window(sf::VideoMode(1200, 600), "Student Center - Login");
    sf::Font font;
    if (!font.loadFromFile("../assets/fonts/OpenSans-Regular.ttf")) {
        std::cerr << "Error loading font.\n";
        return false;
    }

    sf::Text title("Login to Student Center", font, 35);
    title.setPosition(100, 100);
    title.setFillColor(sf::Color::Black);
    title.setStyle(sf::Text::Bold);

    sf::RectangleShape line(sf::Vector2f(900, 2));
    line.setPosition(100, 170);
    line.setFillColor(sf::Color::Black);

    sf::Text userIDLabel("User ID:", font, 24);
    userIDLabel.setPosition(320, 210);
    userIDLabel.setFillColor(sf::Color::Black);
    userIDLabel.setStyle(sf::Text::Bold);

    sf::RectangleShape userIDBackground(sf::Vector2f(325, 30));
    userIDBackground.setPosition(430, 210);
    userIDBackground.setFillColor(sf::Color(230, 230, 250));
    userIDBackground.setOutlineColor(sf::Color::Black);
    userIDBackground.setOutlineThickness(1);

    sf::Text userIDInput("", font, 24);
    userIDInput.setPosition(435, 210);
    userIDInput.setFillColor(sf::Color::Black);

    sf::Text passwordLabel("Password:", font, 24);
    passwordLabel.setPosition(300, 260);
    passwordLabel.setFillColor(sf::Color::Black);
    passwordLabel.setStyle(sf::Text::Bold);

    sf::RectangleShape passwordBackground(sf::Vector2f(325, 30));
    passwordBackground.setPosition(430, 260);
    passwordBackground.setFillColor(sf::Color(230, 230, 250));
    passwordBackground.setOutlineColor(sf::Color::Black);
    passwordBackground.setOutlineThickness(1);

    sf::Text passwordInput("", font, 24);
    passwordInput.setPosition(435, 260);
    passwordInput.setFillColor(sf::Color::Black);

    sf::RectangleShape loginButton(sf::Vector2f(120, 50));
    loginButton.setPosition(550, 310);
    loginButton.setFillColor(sf::Color(128, 0, 128));

    sf::Text loginButtonText("Sign In", font, 24);
    loginButtonText.setPosition(572, 320);
    loginButtonText.setFillColor(sf::Color::White);

    std::string username;
    std::string password;
    bool focusOnUsername = true;

    sf::RectangleShape cursor(sf::Vector2f(1, 20));
    cursor.setFillColor(sf::Color::Black);
    bool cursorVisible = true;
    sf::Clock cursorClock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return false;
            }

            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128) {
                    if (event.text.unicode == '\b') {
                        // Backspace
                        if (focusOnUsername && !username.empty()) {
                            username.pop_back();
                        } else if (!focusOnUsername && !password.empty()) {
                            password.pop_back();
                        }
                    } else {
                        char c = static_cast<char>(event.text.unicode);
                        if (!std::iscntrl((unsigned char)c)) {
                            if (focusOnUsername && username.size() < 20) {
                                username += c;
                            } else if (!focusOnUsername && password.size() < 20) {
                                password += c;
                            }
                        }
                    }
                    userIDInput.setString(username);
                    passwordInput.setString(password);
                }
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);
                if (userIDBackground.getGlobalBounds().contains(mousePos)) {
                    focusOnUsername = true;
                } else if (passwordBackground.getGlobalBounds().contains(mousePos)) {
                    focusOnUsername = false;
                } else if (loginButton.getGlobalBounds().contains(mousePos)) {
                    // Attempt login
                    if (auth->login(trim(username), trim(password))) {
                        std::cout << "Login successful!\n";
                        currentStudent = new Student(1, trim(username), trim(username) + "@example.com", "active");
                        window.close();
                        return true;
                    } else {
                        std::cerr << "Authentication failed.\n";
                    }
                }
            }
        }

        // Cursor blinking
        if (cursorClock.getElapsedTime().asMilliseconds() > 500) {
            cursorVisible = !cursorVisible;
            cursorClock.restart();
        }

        // Update cursor position
        if (focusOnUsername) {
            cursor.setPosition(userIDInput.getPosition().x + userIDInput.getLocalBounds().width + 5,
                               userIDInput.getPosition().y + 5);
        } else {
            cursor.setPosition(passwordInput.getPosition().x + passwordInput.getLocalBounds().width + 5,
                               passwordInput.getPosition().y + 5);
        }

        window.clear(sf::Color::White);
        window.draw(title);
        window.draw(line);
        window.draw(userIDLabel);
        window.draw(userIDBackground);
        window.draw(userIDInput);
        window.draw(passwordLabel);
        window.draw(passwordBackground);
        window.draw(passwordInput);
        window.draw(loginButton);
        window.draw(loginButtonText);

        if (cursorVisible) {
            window.draw(cursor);
        }

        window.display();
    }

    return false;
}

void browseCourses() {
    sf::RenderWindow browseWindow(sf::VideoMode(800, 600), "Browse Courses");
    sf::Font font;
    if (!font.loadFromFile("../assets/fonts/OpenSans-Regular.ttf")) {
        std::cerr << "Error loading font.\n";
        return;
    }

    std::vector<Course*> courses = courseManager.getCourseList();
    std::vector<sf::Text> courseTexts;
    int yPosition = 50;
    for (auto* course : courses) {
        sf::Text courseText;
        courseText.setFont(font);
        courseText.setString("ID: " + std::to_string(course->getCourseID()) + " Name: " + course->getCourseName());
        courseText.setCharacterSize(20);
        courseText.setPosition(50, yPosition);
        courseText.setFillColor(sf::Color::Black);
        courseTexts.push_back(courseText);
        yPosition += 30;
    }

    int scrollOffset = 0;
    while (browseWindow.isOpen()) {
        sf::Event event;
        while (browseWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                browseWindow.close();
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    scrollOffset = std::max(0, scrollOffset - 20);
                } else if (event.key.code == sf::Keyboard::Down) {
                    scrollOffset += 20;
                }
            }
        }

        browseWindow.clear(sf::Color::White);
        for (auto& courseText : courseTexts) {
            courseText.move(0, -scrollOffset);
            browseWindow.draw(courseText);
            courseText.move(0, scrollOffset);
        }
        browseWindow.display();
    }
}

void searchCourses() {
    std::string keyword;
    std::cout << "\n--- Search Courses ---\n";
    std::cout << "Enter keyword: ";
    std::cin >> keyword;

    auto results = courseManager.filterCoursesByDepartment(keyword);
    if (results.empty()) {
        std::cout << "No courses found for \"" << keyword << "\".\n";
    } else {
        std::cout << "Courses matching \"" << keyword << "\":\n";
        for (const auto& course : results) {
            std::cout << "ID: " << course->getCourseID()
                      << ", Name: " << course->getCourseName()
                      << ", Schedule: " << course->getSchedule()
                      << ", Capacity: " << course->getCapacity() << std::endl;
        }
    }
}

void viewProfile() {
    if (!currentStudent) {
        std::cout << "No student logged in.\n";
        return;
    }

    std::cout << "\n--- Student Profile ---\n";
    std::cout << "ID: " << currentStudent->getStudentID() << "\n";
    std::cout << "Name: " << currentStudent->getUsername() << "\n";
    std::cout << "Email: " << currentStudent->getEmail() << "\n";
    currentStudent->listCourses();
}

void manageEnrollment() {
    if (!currentStudent) {
        std::cout << "No student logged in.\n";
        return;
    }

    std::cout << "\n--- Manage Enrollment ---\n";
    std::cout << "1. Enroll in a course\n";
    std::cout << "2. Drop a course\n";
    std::cout << "3. View draft schedule\n";
    std::cout << "4. Finalize schedule\n";

    int choice;
    std::cin >> choice;

    if (choice == 1) {
        int courseID;
        std::cout << "Enter Course ID to enroll: ";
        std::cin >> courseID;
        if (scheduler.scheduleCourse(currentStudent, courseID)) {
            std::cout << "Enrollment successful.\n";
        } else {
            std::cout << "Enrollment failed.\n";
        }
    } else if (choice == 2) {
        int courseID;
        std::cout << "Enter Course ID to drop: ";
        std::cin >> courseID;
        currentStudent->dropCourse(courseID);
    } else if (choice == 3) {
        scheduler.generateSchedule(currentStudent);
    } else if (choice == 4) {
        if (scheduler.finalizeDraftSchedule(currentStudent)) {
            std::cout << "Schedule finalized successfully.\n";
        } else {
            std::cout << "Schedule finalization failed.\n";
        }
    } else {
        std::cout << "Invalid choice.\n";
    }
}

bool showMainMenu() {
    if (!currentStudent) {
        std::cerr << "Error: No logged-in student available for Main Menu.\n";
        return false;
    }

    showMainPage();
    return true;
}

void showMainPage() {
    if (!currentStudent) {
        std::cerr << "Error: No logged-in student available for Main Page.\n";
        return;
    }

    sf::RenderWindow window(sf::VideoMode(800, 600), "Student Center - Main Page");
    sf::Font font;
    if (!font.loadFromFile("../assets/fonts/OpenSans-Regular.ttf")) {
        std::cerr << "Error loading font.\n";
        return;
    }

    sf::Text title("Main Menu", font, 40);
    title.setPosition(300, 50);
    title.setFillColor(sf::Color::Black);

    sf::RectangleShape browseButton(sf::Vector2f(200, 50));
    browseButton.setPosition(300, 150);
    browseButton.setFillColor(sf::Color(128, 0, 128));
    sf::Text browseText("Browse Courses", font, 24);
    browseText.setPosition(320, 160);
    browseText.setFillColor(sf::Color::White);

    sf::RectangleShape searchButton(sf::Vector2f(200, 50));
    searchButton.setPosition(300, 220);
    searchButton.setFillColor(sf::Color(128, 0, 128));
    sf::Text searchText("Search Courses", font, 24);
    searchText.setPosition(320, 230);
    searchText.setFillColor(sf::Color::White);

    sf::RectangleShape profileButton(sf::Vector2f(200, 50));
    profileButton.setPosition(300, 290);
    profileButton.setFillColor(sf::Color(128, 0, 128));
    sf::Text profileText("View Profile", font, 24);
    profileText.setPosition(320, 300);
    profileText.setFillColor(sf::Color::White);

    sf::RectangleShape logoutButton(sf::Vector2f(200, 50));
    logoutButton.setPosition(300, 360);
    logoutButton.setFillColor(sf::Color(128, 0, 128));
    sf::Text logoutText("Logout", font, 24);
    logoutText.setPosition(350, 370);
    logoutText.setFillColor(sf::Color::White);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);

                if (browseButton.getGlobalBounds().contains(mousePos)) {
                    browseCourses();
                } else if (searchButton.getGlobalBounds().contains(mousePos)) {
                    searchCourses();
                } else if (profileButton.getGlobalBounds().contains(mousePos)) {
                    viewProfile();
                } else if (logoutButton.getGlobalBounds().contains(mousePos)) {
                    std::cout << "Logging out...\n";
                    delete currentStudent;
                    currentStudent = nullptr;
                    window.close();
                }
            }
        }

        window.clear(sf::Color::White);
        window.draw(title);
        window.draw(browseButton);
        window.draw(browseText);
        window.draw(searchButton);
        window.draw(searchText);
        window.draw(profileButton);
        window.draw(profileText);
        window.draw(logoutButton);
        window.draw(logoutText);
        window.display();
    }
}
