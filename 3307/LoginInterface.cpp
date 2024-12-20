#include "LoginInterface.h"
#include "Authentication.h"
#include "Student.h"
#include "Utils.h"
#include "NotificationInterface.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cctype>

extern Student* currentStudent;
extern Authentication* auth;


/**
 * Displays the login interface and handles user interaction for logging in.
 *
 * @return True if login is successful and the window is closed, false if the window is closed without logging in.
 */
bool LoginInterface::show() {
    // Page initialization
    sf::RenderWindow window(sf::VideoMode(1200, 600), "Student Center - Login");
    sf::Font font;
    if(!font.loadFromFile("../assets/fonts/OpenSans-Regular.ttf")) return false;

    // Main Title
    sf::Text title("Login to Student Center", font, 35);
    title.setPosition(100,100);
    title.setFillColor(sf::Color::Black);

    sf::RectangleShape line(sf::Vector2f(900,2));
    line.setPosition(100,160);
    line.setFillColor(sf::Color::Black);

    // ID input section
    sf::Text userIDLabel("User ID:", font, 24);
    userIDLabel.setPosition(320,210);
    userIDLabel.setFillColor(sf::Color::Black);

    sf::RectangleShape userIDBackground(sf::Vector2f(325,30));
    userIDBackground.setPosition(430,210);
    userIDBackground.setFillColor(sf::Color(230,230,250));
    userIDBackground.setOutlineColor(sf::Color::Black);
    userIDBackground.setOutlineThickness(1);

    sf::Text userIDInput("", font, 24);
    userIDInput.setPosition(435,210);
    userIDInput.setFillColor(sf::Color::Black);

    sf::Text passwordLabel("Password:", font, 24);
    passwordLabel.setPosition(300,260);
    passwordLabel.setFillColor(sf::Color::Black);

    sf::RectangleShape passwordBackground(sf::Vector2f(325,30));
    passwordBackground.setPosition(430,260);
    passwordBackground.setFillColor(sf::Color(230,230,250));
    passwordBackground.setOutlineColor(sf::Color::Black);
    passwordBackground.setOutlineThickness(1);

    sf::Text passwordInput("", font, 24);
    passwordInput.setPosition(435,260);
    passwordInput.setFillColor(sf::Color::Black);

    // Sign in button
    sf::RectangleShape loginButton(sf::Vector2f(120,50));
    loginButton.setPosition(550,310);
    loginButton.setFillColor(sf::Color(128,0,128));

    sf::Text loginButtonText("Sign In", font,24);
    loginButtonText.setPosition(572,320);
    loginButtonText.setFillColor(sf::Color::White);

    std::string username;
    std::string password;
    bool focusOnUsername = true;

    sf::RectangleShape cursor(sf::Vector2f(1,20));
    cursor.setFillColor(sf::Color::Black);
    bool cursorVisible = true;
    sf::Clock cursorClock;

    while (window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                window.close();
                return false;
            }
            if(event.type == sf::Event::TextEntered) {
                if(event.text.unicode < 128) {
                    if(event.text.unicode == '\b') {
                        if(focusOnUsername && !username.empty()) username.pop_back();
                        else if(!focusOnUsername && !password.empty()) password.pop_back();
                    } else {
                        char c = (char)event.text.unicode;
                        if(!std::iscntrl((unsigned char)c)) {
                            if(focusOnUsername && username.size()<20) username += c;
                            else if(!focusOnUsername && password.size()<20) password += c;
                        }
                    }
                    userIDInput.setString(username);
                    passwordInput.setString(password);
                }
            }
            if(event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mp=(sf::Vector2f)sf::Mouse::getPosition(window);
                if(userIDBackground.getGlobalBounds().contains(mp)) {
                    focusOnUsername = true;
                } else if(passwordBackground.getGlobalBounds().contains(mp)) {
                    focusOnUsername = false;
                } else if(loginButton.getGlobalBounds().contains(mp)) {
                    if(auth->login(trim(username), trim(password))) {
                        // Determine student id from username
                        int uid = 0;
                        try {
                            uid = std::stoi(trim(username));
                        } catch (...) {
                            uid = 0; // fallback if not numeric
                        }
                        if (uid == 0) {
                            NotificationInterface::show("Invalid user ID.");
                        } else {
                            currentStudent = new Student(uid, trim(username), trim(username)+"@example.com","active");
                            window.close();
                            return true;
                        }
                    } else NotificationInterface::show("Authentication failed.");
                }
            }
        }
        if(cursorClock.getElapsedTime().asMilliseconds()>500) {
            cursorVisible=!cursorVisible;
            cursorClock.restart();
        }
        if(focusOnUsername)
            cursor.setPosition(userIDInput.getPosition().x+userIDInput.getLocalBounds().width+5,userIDInput.getPosition().y+5);
        else
            cursor.setPosition(passwordInput.getPosition().x+passwordInput.getLocalBounds().width+5,passwordInput.getPosition().y+5);

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
        if(cursorVisible) window.draw(cursor);
        window.display();
    }
    return false;
}
