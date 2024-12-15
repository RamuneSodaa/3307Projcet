#include "MainInterface.h"
#include "Authentication.h"
#include "CourseManager.h"
#include "Scheduler.h"
#include "Student.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

// Shared objects
Authentication* auth = Authentication::getInstance();
CourseManager courseManager;
Scheduler scheduler("Fall 2024");
extern Student* currentStudent;

// Login functionality
void login() {
    std::string username, password;
    std::cout << "\n--- Login ---\n";
    std::cout << "Username: ";
    std::cin >> username;
    std::cout << "Password: ";
    std::cin >> password;

    if (auth->login(username, password)) {
        currentStudent = new Student(1, username, username + "@example.com", "active");
        std::cout << "Login successful! Welcome, " << username << ".\n";
    } else {
        std::cout << "Invalid credentials. Please try again.\n";
    }
}

extern CourseManager courseManager;

void browseCourses() {
    sf::RenderWindow browseWindow(sf::VideoMode(800, 600), "Browse Courses");
    sf::Font font;
    if (!font.loadFromFile("../assets/fonts/OpenSans-Regular.ttf")) {
        std::cerr << "Error loading font.\n";
        return;
    }

    std::vector<Course*> courses = courseManager.getCourseList(); // Assuming getCourseList() provides access to courses
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


// Search courses by keyword
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

// View student's profile
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

// Manage enrollment
void manageEnrollment() {
    if (!currentStudent) {
        std::cout << "No student logged in.\n";
        return;
    }

    int choice;
    std::cout << "\n--- Manage Enrollment ---\n";
    std::cout << "1. Enroll in a course\n";
    std::cout << "2. Drop a course\n";
    std::cout << "3. View draft schedule\n";
    std::cout << "4. Finalize schedule\n";
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

// Show the main graphical page
void showMainPage() {
    if (!currentStudent) {
        std::cerr << "Error: No logged-in student available for Main Page.\n";
        return;
    }

    // Create SFML window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Student Center - Main Page");

    // Load font
    sf::Font font;
    if (!font.loadFromFile("../assets/fonts/OpenSans-Regular.ttf")) {
        std::cerr << "Error loading font.\n";
        return;
    }

    // Title
    sf::Text title("Main Menu", font, 40);
    title.setPosition(300, 50);
    title.setFillColor(sf::Color::Black);

    // Buttons
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

    // Main event loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));

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

// Update showMainMenu to call showMainPage
bool showMainMenu() {
    if (!currentStudent) {
        std::cerr << "Error: No logged-in student available for Main Menu.\n";
        return false;
    }

    showMainPage();
    return true;
}

// Logout functionality
void logout(sf::RenderWindow& window) {
    std::cout << "Logging out...\n";
    delete currentStudent; // Free the memory
    currentStudent = nullptr; // Reset the pointer
    window.close(); // Close the main menu window
}
