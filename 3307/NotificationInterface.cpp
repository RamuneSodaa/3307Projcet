#include "NotificationInterface.h"
#include <SFML/Graphics.hpp>
#include <iostream>

void NotificationInterface::show(const std::string& message) {
    sf::RenderWindow window(sf::VideoMode(400, 200), "Notification", sf::Style::Close);
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("../assets/fonts/OpenSans-Regular.ttf")) {
        std::cerr << "Failed to load font for notification." << std::endl;
        return;
    }

    sf::Text msg(message, font, 20);
    msg.setFillColor(sf::Color::Black);
    msg.setPosition(20, 60);

    sf::RectangleShape closeButton(sf::Vector2f(80,30));
    closeButton.setPosition((400 - 80) / 2.0f, 140);
    closeButton.setFillColor(sf::Color(128,0,128));

    sf::Text closeText("Close", font, 18);
    closeText.setFillColor(sf::Color::White);
    closeText.setPosition(closeButton.getPosition().x + 17, closeButton.getPosition().y + 3);

    while (window.isOpen()) {
        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) window.close();
            if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mp = (sf::Vector2f)sf::Mouse::getPosition(window);
                if (closeButton.getGlobalBounds().contains(mp)) {
                    window.close();
                }
            }
        }

        window.clear(sf::Color::White);
        window.draw(msg);
        window.draw(closeButton);
        window.draw(closeText);
        window.display();
    }
}
