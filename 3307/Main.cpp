#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <string>
#include "Authentication.h"
#include "MainInterface.h"
#include "Student.h"
#include "CourseManager.h"

Student* currentStudent = nullptr;

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
    // Initialize SFML window
    sf::RenderWindow window(sf::VideoMode(1200, 600), "Student Center");
    sf::Font font;
    CourseManager courseManager;
    if (!font.loadFromFile("../assets/fonts/OpenSans-Regular.ttf")) {
        std::cerr << "Error loading font.\n";
        return -1;
    }

    // Set up Authentication
    Authentication* auth = Authentication::getInstance();
    auth->addUser("1", "2"); // Example user

    // UI Components
    sf::Text mfaText("Login to Student Center", font, 35);
    mfaText.setPosition(100, 100);
    mfaText.setFillColor(sf::Color::Black);

    sf::Text userIDLabel("User ID:", font, 24);
    userIDLabel.setPosition(300, 210);
    userIDLabel.setFillColor(sf::Color::Black);

    sf::Text passwordLabel("Password:", font, 24);
    passwordLabel.setPosition(300, 260);
    passwordLabel.setFillColor(sf::Color::Black);

    sf::RectangleShape userIDBackground(sf::Vector2f(350, 30));
    userIDBackground.setPosition(405, 210);
    userIDBackground.setFillColor(sf::Color(230, 230, 250));
    userIDBackground.setOutlineColor(sf::Color::Black);
    userIDBackground.setOutlineThickness(1);

    sf::RectangleShape passwordBackground(sf::Vector2f(350, 30));
    passwordBackground.setPosition(405, 260);
    passwordBackground.setFillColor(sf::Color(230, 230, 250));
    passwordBackground.setOutlineColor(sf::Color::Black);
    passwordBackground.setOutlineThickness(1);

    sf::Text userIDInput("", font, 24);
    userIDInput.setPosition(410, 210);
    userIDInput.setFillColor(sf::Color::Black);

    sf::Text passwordInput("", font, 24);
    passwordInput.setPosition(410, 260);
    passwordInput.setFillColor(sf::Color::Black);

    sf::RectangleShape loginButton(sf::Vector2f(120, 50));
    loginButton.setPosition(550, 310);
    loginButton.setFillColor(sf::Color(128, 0, 128));

    sf::Text loginButtonText("Sign In", font, 24);
    loginButtonText.setPosition(572, 320);
    loginButtonText.setFillColor(sf::Color::White);

    sf::Text helpText("Need help in login? Click HERE", font, 20);
    helpText.setPosition(100, 380);
    helpText.setFillColor(sf::Color::Blue);

    sf::Text mfaContactText("Have feedback or complaints? Click HERE", font, 20);
    mfaContactText.setPosition(100, 410);
    mfaContactText.setFillColor(sf::Color::Blue);

    // Variables to store input and focus
    std::string username = "";
    std::string password = "";
    bool focusOnUsername = true;

    sf::RectangleShape cursor(sf::Vector2f(2, 20)); // Blinking cursor
    cursor.setFillColor(sf::Color::Black);
    bool cursorVisible = true;
    sf::Clock cursorClock;


    // Main event loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Handle text input
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode >= 32 && event.text.unicode <= 126) { // Printable characters
                    char enteredChar = static_cast<char>(event.text.unicode);

                    if (focusOnUsername && username.length() < 20) {
                        username += enteredChar;
                    } else if (!focusOnUsername && password.length() < 20) {
                        password += enteredChar;
                    }

                    userIDInput.setString(username);
                    passwordInput.setString(password);
                }

                // Handle backspace
                if (event.text.unicode == '\b') { // Backspace key
                    if (focusOnUsername && !username.empty()) {
                        username.pop_back();
                    } else if (!focusOnUsername && !password.empty()) {
                        password.pop_back();
                    }

                    userIDInput.setString(username);
                    passwordInput.setString(password);
                }
            }

            // Handle tab switching focus
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Tab) {
                focusOnUsername = !focusOnUsername; // Toggle focus between username and password
            }

            // Handle mouse clicks
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));

                if (userIDBackground.getGlobalBounds().contains(mousePos)) {
                    focusOnUsername = true;
                } else if (passwordBackground.getGlobalBounds().contains(mousePos)) {
                    focusOnUsername = false;
                } else if (loginButton.getGlobalBounds().contains(mousePos)) {
                    if (auth->login(trim(username), trim(password))) {
                        std::cout << "Login successful for user: " << username << "\n";
                        currentStudent = new Student(1, username, username + "@example.com", "active");
                        window.close();
                        // Initialize course manager
                        courseManager.initializeCourses();
                        // Call browseCourses before showing the main menu
                        courseManager.browseCoursesUI(window, font);
                        if (!showMainMenu()) {
                            std::cerr << "Error: Could not display Main Menu.\n";
                        }
                        delete currentStudent;
                        currentStudent = nullptr;
                    } else {
                        std::cerr << "Invalid credentials. Please try again.\n";
                    }
                }
            }
        }

        // Cursor blinking logic
        if (focusOnUsername) {
            cursor.setPosition(userIDInput.getPosition().x + userIDInput.getLocalBounds().width + 5, userIDInput.getPosition().y + 5);
        } else {
            cursor.setPosition(passwordInput.getPosition().x + passwordInput.getLocalBounds().width + 5, passwordInput.getPosition().y + 5);
        }

        if (cursorClock.getElapsedTime().asMilliseconds() > 500) {
            cursorVisible = !cursorVisible;
            cursorClock.restart();
        }

        // Render the window
        window.clear(sf::Color::White);
        window.draw(mfaText);
        window.draw(userIDLabel);
        window.draw(passwordLabel);
        window.draw(userIDBackground);
        window.draw(passwordBackground);
        window.draw(userIDInput);
        window.draw(passwordInput);
        window.draw(loginButton);
        window.draw(loginButtonText);
        window.draw(helpText);
        window.draw(mfaContactText);

        if (cursorVisible) {
            window.draw(cursor);
        }

        window.display();
    }

    return 0;
}
