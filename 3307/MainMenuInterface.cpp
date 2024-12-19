#include "MainMenuInterface.h"
#include "Authentication.h"
#include "Student.h"
#include <iostream>
#include <algorithm>
#include <cctype>

extern Student* currentStudent;
extern Authentication* auth;

MainMenuInterface::MainMenuInterface(CourseManager& cm, Scheduler& sched, EnrollmentManager& em)
: courseManager(cm), scheduler(sched), enrollmentManager(em), window(sf::VideoMode(1280, 720), "Student Center - Main Page") {
    if (!font.loadFromFile("../assets/fonts/OpenSans-Regular.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
    }
}

void MainMenuInterface::show() {
    if (!currentStudent) return;

    float offsetY = 50.0f;

    sf::Image icon;
    if (icon.loadFromFile("../assets/icon.png")) {
        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    }

    sf::Text title("Main Menu", font, 40);
    title.setPosition(100, 50);
    title.setFillColor(sf::Color::Black);

    sf::RectangleShape line(sf::Vector2f(1050, 2));
    line.setPosition(100, 110);
    line.setFillColor(sf::Color::Black);

    // Profile image and outline
    sf::Texture picTex;
    sf::Sprite picSprite;
    sf::RectangleShape picOutline;
    if (picTex.loadFromFile("../assets/user_profile.png")) {
        picSprite.setTexture(picTex);
        picSprite.setPosition(130, 150);
        picSprite.setScale(0.13f, 0.13f);

        sf::FloatRect picBounds = picSprite.getGlobalBounds();
        picOutline.setSize(sf::Vector2f(picBounds.width + 8, picBounds.height + 8));
        picOutline.setPosition(picBounds.left - 4, picBounds.top - 4);
        picOutline.setFillColor(sf::Color::Transparent);
        picOutline.setOutlineColor(sf::Color::Black);
        picOutline.setOutlineThickness(5);
    }

    sf::Text infoText(
        "ID: " + std::to_string(currentStudent->getStudentID()) +
        "\nName: " + currentStudent->getUsername() +
        "\nEmail: " + currentStudent->getEmail(),
        font, 24
    );
    infoText.setPosition(100, 295 + offsetY);
    infoText.setFillColor(sf::Color::Black);

    sf::RectangleShape searchButton(sf::Vector2f(200, 50));
    searchButton.setPosition(100, 430 + offsetY);
    searchButton.setFillColor(sf::Color(128, 0, 128));

    sf::Text searchText("Search Courses", font, 24);
    searchText.setPosition(120, 440 + offsetY);
    searchText.setFillColor(sf::Color::White);

    sf::RectangleShape logoutButton(sf::Vector2f(200, 50));
    logoutButton.setPosition(100, 500 + offsetY);
    logoutButton.setFillColor(sf::Color(128, 0, 128));

    sf::Text logoutText("Logout", font, 24);
    logoutText.setPosition(160, 510 + offsetY);
    logoutText.setFillColor(sf::Color::White);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mp(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
                if (searchButton.getGlobalBounds().contains(mp)) {
                    window.close();
                    showSearchPage();
                }
                if (logoutButton.getGlobalBounds().contains(mp)) {
                    auth->logout(currentStudent->getUsername());
                    currentStudent = nullptr;
                    window.close();
                    std::cout << "User logged out.\n";
                }
            }
        }

        window.clear(sf::Color::White);
        window.draw(title);
        window.draw(line);
        window.draw(picSprite);
        window.draw(picOutline);
        window.draw(infoText);
        window.draw(searchButton);
        window.draw(searchText);
        window.draw(logoutButton);
        window.draw(logoutText);
        window.display();
    }
}

void MainMenuInterface::showSearchPage() {
    sf::RenderWindow searchWindow(sf::VideoMode(1280, 720), "Student Center - Search Courses");
    sf::Text title("Search Courses", font, 40);
    title.setPosition(100, 50);
    title.setFillColor(sf::Color::Black);

    sf::RectangleShape line(sf::Vector2f(1050, 2));
    line.setPosition(100, 110);
    line.setFillColor(sf::Color::Black);

    sf::Text searchLabel("Enter Course ID or Name:", font, 24);
    searchLabel.setPosition(100, 150);
    searchLabel.setFillColor(sf::Color::Black);

    sf::RectangleShape searchBar(sf::Vector2f(500, 30));
    searchBar.setPosition(100, 190);
    searchBar.setFillColor(sf::Color(230,230,250));
    searchBar.setOutlineColor(sf::Color::Black);
    searchBar.setOutlineThickness(1);

    sf::Text searchInput("", font, 24);
    searchInput.setPosition(105, 190);
    searchInput.setFillColor(sf::Color::Black);

    sf::RectangleShape cursor(sf::Vector2f(1, 24));
    cursor.setFillColor(sf::Color::Black);
    bool cursorVisible = true;
    sf::Clock cursorClock;

    sf::RectangleShape doSearchButton(sf::Vector2f(120, 50));
    doSearchButton.setPosition(620, 180);
    doSearchButton.setFillColor(sf::Color(128,0,128));

    sf::Text doSearchButtonText("Search", font, 24);
    doSearchButtonText.setPosition(640, 190);
    doSearchButtonText.setFillColor(sf::Color::White);

    std::string query;
    bool focusOnSearchBar = false;
    std::vector<Course*> searchResults;

    struct EnrollButton {
        sf::RectangleShape shape;
        sf::Text text;
        int courseID;
    };
    std::vector<EnrollButton> enrollButtons;

    auto performSearch = [&](const std::string& q) {
        searchResults.clear();
        enrollButtons.clear();

        std::string trimmed = q;
        // Trim spaces
        trimmed.erase(trimmed.begin(), std::find_if(trimmed.begin(), trimmed.end(),
                    [](unsigned char ch){ return !std::isspace(ch); }));
        trimmed.erase(std::find_if(trimmed.rbegin(), trimmed.rend(),
                    [](unsigned char ch){ return !std::isspace(ch); }).base(), trimmed.end());

        if (trimmed.empty()) return;

        std::cout << "Search button pressed. Query: " << trimmed << "\n";
        std::cout << "Searching for: " << trimmed << "\n";

        std::vector<Course*> results = courseManager.searchCourses(trimmed);
        std::cout << "Found " << results.size() << " results.\n";

        searchResults.insert(searchResults.end(), results.begin(), results.end());

        float yPos = 245.0f;
        for (auto c : searchResults) {
            EnrollButton eb;
            eb.shape.setSize(sf::Vector2f(80, 30));
            eb.shape.setFillColor(sf::Color(0, 128, 0)); // Green
            eb.shape.setPosition(900, yPos);
            eb.text.setFont(font);
            eb.text.setString("Enroll");
            eb.text.setCharacterSize(20);
            eb.text.setFillColor(sf::Color::White);
            eb.text.setPosition(eb.shape.getPosition().x + 10, eb.shape.getPosition().y + 2);
            eb.courseID = c->getCourseID();

            enrollButtons.push_back(eb);
            yPos += 40;
        }
    };

    float resultsStartY = 250.0f;

    while (searchWindow.isOpen()) {
        sf::Event event;
        while (searchWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                searchWindow.close();
            }

            if (event.type == sf::Event::TextEntered && focusOnSearchBar) {
                if (event.text.unicode < 128) {
                    char c = static_cast<char>(event.text.unicode);
                    if (c == '\b') {
                        if (!query.empty()) query.pop_back();
                    } else if (!std::iscntrl((unsigned char)c)) {
                        if (query.size() < 50) {
                            query += c;
                        }
                    }
                    searchInput.setString(query);
                }
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mp((float)event.mouseButton.x, (float)event.mouseButton.y);
                if (searchBar.getGlobalBounds().contains(mp)) {
                    focusOnSearchBar = true;
                } else {
                    focusOnSearchBar = false;
                }

                if (doSearchButton.getGlobalBounds().contains(mp)) {
                    // Perform search with the current query
                    performSearch(query);
                }

                // Check if enroll button clicked
                for (auto &eb : enrollButtons) {
                    if (eb.shape.getGlobalBounds().contains(mp)) {
                        if (enrollmentManager.enrollStudentInCourse(currentStudent, eb.courseID)) {
                            std::cout << "Enrolled in course ID: " << eb.courseID << "\n";
                        } else {
                            std::cout << "Failed to enroll in course ID: " << eb.courseID << "\n";
                        }
                    }
                }
            }
        }

        // Handle cursor blinking
        if (cursorClock.getElapsedTime().asMilliseconds() > 500) {
            cursorVisible = !cursorVisible;
            cursorClock.restart();
        }

        float textWidth = searchInput.getLocalBounds().width;
        cursor.setPosition(searchInput.getPosition().x + textWidth + 5, searchInput.getPosition().y + 3);

        searchWindow.clear(sf::Color::White);
        searchWindow.draw(title);
        searchWindow.draw(line);
        searchWindow.draw(searchLabel);
        searchWindow.draw(searchBar);
        searchWindow.draw(searchInput);
        if (focusOnSearchBar && cursorVisible) searchWindow.draw(cursor);
        searchWindow.draw(doSearchButton);
        searchWindow.draw(doSearchButtonText);

        float yPos = resultsStartY;
        if (!searchResults.empty()) {
            for (size_t i = 0; i < searchResults.size(); ++i) {
                Course* c = searchResults[i];
                sf::Text courseInfo("ID: " + std::to_string(c->getCourseID()) +
                                    " | Name: " + c->getCourseName() +
                                    " | Schedule: " + c->getSchedule(), font, 20);
                courseInfo.setPosition(100, yPos);
                courseInfo.setFillColor(sf::Color::Black);
                searchWindow.draw(courseInfo);

                auto &eb = enrollButtons[i];
                searchWindow.draw(eb.shape);
                searchWindow.draw(eb.text);

                yPos += 40;
            }
        } else {
            if (!query.empty()) {
                sf::Text noResults("No results found.", font, 24);
                noResults.setPosition(100, yPos);
                noResults.setFillColor(sf::Color::Black);
                searchWindow.draw(noResults);
            }
        }

        searchWindow.display();
    }
}
