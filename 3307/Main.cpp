#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <chrono>
#include "Authentication.h"

// Logo creating prompt: https://chatgpt.com/share/6756e190-0414-8006-8cb6-5458b7d35882

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
    sf::RenderWindow window(sf::VideoMode(1200, 600), "Student Center");
    sf::Font font;

    sf::Image icon;
    if (icon.loadFromFile("../assets/icon.png")) {
        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    }

    if (!font.loadFromFile("../assets/fonts/OpenSans-Regular.ttf")) {
        std::cerr << "Error loading font.\n";
        return -1;
    }

    Authentication* auth = Authentication::getInstance();
    auth->addUser("XFENG282", "password123");

    sf::Text mfaText("Login to Student Center", font, 35);
    mfaText.setPosition(100, 100);
    mfaText.setFillColor(sf::Color::Black);
    mfaText.setStyle(sf::Text::Bold);

    sf::RectangleShape line(sf::Vector2f(900, 2));
    line.setPosition(100, 170);
    line.setFillColor(sf::Color::Black);

    sf::Text userIDLabel("User ID:", font, 24);
    userIDLabel.setPosition(300, 210);
    userIDLabel.setFillColor(sf::Color::Black);
    userIDLabel.setStyle(sf::Text::Bold);

    sf::RectangleShape userIDBackground(sf::Vector2f(350, 30));
    userIDBackground.setPosition(405, 210);
    userIDBackground.setFillColor(sf::Color(230, 230, 250));
    userIDBackground.setOutlineColor(sf::Color::Black);
    userIDBackground.setOutlineThickness(1);

    sf::Text userIDInput("", font, 24);
    userIDInput.setPosition(410, 210);
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

    std::string username = "";
    std::string password = "";
    bool focusOnUsername = true;

    sf::Text helpText("Need help in login? Click HERE", font, 20);
    helpText.setPosition(100, 380);
    helpText.setFillColor(sf::Color::Blue);

    sf::Text mfaContactText("Have feedback or complains? Click HERE", font, 20);
    mfaContactText.setPosition(100, 410);
    mfaContactText.setFillColor(sf::Color::Blue);

    // Cursor setup
    sf::RectangleShape cursor(sf::Vector2f(2, 20)); // Width = 2px, height = text height
    cursor.setFillColor(sf::Color::Black);
    bool cursorVisible = true; // Blinking state
    sf::Clock cursorClock;     // Clock for blinking

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128) {
                    if (event.text.unicode == '\b') { // Handle backspace
                        if (focusOnUsername && !username.empty()) {
                            username.pop_back();
                        } else if (!focusOnUsername && !password.empty()) {
                            password.pop_back();
                        }
                    } else { // Add new character
                        char enteredChar = static_cast<char>(event.text.unicode);
                        if (focusOnUsername && username.length() < 20) {
                            username += enteredChar;
                        } else if (!focusOnUsername && password.length() < 20) {
                            password += enteredChar;
                        }
                    }
                }
                userIDInput.setString(username);
                passwordInput.setString(password);
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
                if (userIDBackground.getGlobalBounds().contains(mousePos)) {
                    focusOnUsername = true;
                } else if (passwordBackground.getGlobalBounds().contains(mousePos)) {
                    focusOnUsername = false;
                } else if (loginButton.getGlobalBounds().contains(mousePos)) {
                    if (auth->login(trim(username), trim(password))) {
                        std::cout << "Login successful!\n";
                    } else {
                        std::cerr << "Authentication failed.\n";
                    }
                }
            }
        }

        // Update cursor position
        if (focusOnUsername) {
            cursor.setPosition(userIDInput.getPosition().x + userIDInput.getLocalBounds().width + 5, userIDInput.getPosition().y + 5);
        } else {
            cursor.setPosition(passwordInput.getPosition().x + passwordInput.getLocalBounds().width + 5, passwordInput.getPosition().y + 5);
        }

        // Handle cursor blinking
        if (cursorClock.getElapsedTime().asMilliseconds() > 500) {
            cursorVisible = !cursorVisible;
            cursorClock.restart();
        }

        window.clear(sf::Color::White);
        window.draw(mfaText);
        window.draw(line);
        window.draw(userIDLabel);
        window.draw(userIDBackground);
        window.draw(userIDInput);
        window.draw(passwordLabel);
        window.draw(passwordBackground);
        window.draw(passwordInput);
        window.draw(loginButton);
        window.draw(loginButtonText);
        window.draw(helpText);
        window.draw(mfaContactText);

        // Draw cursor if visible
        if (cursorVisible) {
            window.draw(cursor);
        }

        window.display();
    }

    return 0;
}
