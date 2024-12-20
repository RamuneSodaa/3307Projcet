#include "MainMenuInterface.h"
#include "Authentication.h"
#include "Student.h"
#include "DatabaseManager.h"
#include "EnrollmentManager.h"
#include <iostream>
#include <algorithm>
#include <cctype>

extern Student* currentStudent;
extern Authentication* auth;

/**
 * Constructs the MainMenuInterface with references to course manager, scheduler, and enrollment manager.
 * Initializes the main window with a predefined video mode and title.
 *
 * @param cm Reference to the CourseManager managing the courses.
 * @param sched Reference to the Scheduler managing the scheduling of courses.
 * @param em Reference to the EnrollmentManager managing student enrollments.
 */

MainMenuInterface::MainMenuInterface(CourseManager& cm, Scheduler& sched, EnrollmentManager& em)
: courseManager(cm), scheduler(sched), enrollmentManager(em), window(sf::VideoMode(1280, 720), "Student Center - Main Page") {
    if (!font.loadFromFile("../assets/fonts/OpenSans-Regular.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
    }
}

/**
 * Displays the main menu interface, allowing students to view enrolled courses, search for courses,
 * and logout. The interface updates dynamically based on user interactions such as searching and logging out.
 */
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

    std::vector<EnrollmentRecord> enrollmentRecords = DatabaseManager::getEnrollmentsForStudent(currentStudent->getStudentID());

    float tableStartY = 150.0f;
    float tableStartX = 400.0f;

    sf::Text tableTitle("Enrolled Courses", font, 30);
    tableTitle.setFillColor(sf::Color::Black);
    tableTitle.setPosition(tableStartX, tableStartY);

    sf::Text colCourseID("Course ID", font, 20);
    colCourseID.setFillColor(sf::Color::Black);
    colCourseID.setPosition(tableStartX, tableStartY + 40);

    sf::Text colCourseName("Course Name", font, 20);
    colCourseName.setFillColor(sf::Color::Black);
    colCourseName.setPosition(tableStartX+150, tableStartY + 40);

    sf::Text colEnrollmentDate("Enrollment Date", font, 20);
    colEnrollmentDate.setFillColor(sf::Color::Black);
    colEnrollmentDate.setPosition(tableStartX+300, tableStartY + 40);

    sf::Text colAction("Action", font, 20);
    colAction.setFillColor(sf::Color::Black);
    colAction.setPosition(tableStartX+480, tableStartY + 40);

    sf::RectangleShape headerLine(sf::Vector2f(650, 2));
    headerLine.setPosition(tableStartX, tableStartY + 70);
    headerLine.setFillColor(sf::Color::Black);

    struct DropButton {
        sf::RectangleShape shape;
        sf::Text text;
        int courseID;
    };
    std::vector<DropButton> dropButtons;

    auto rebuildDropButtons = [&](const std::vector<EnrollmentRecord>& records) {
        dropButtons.clear();
        float yPos = tableStartY + 90;
        for (auto& rec : records) {
            DropButton db;
            db.shape.setSize(sf::Vector2f(80, 30));
            db.shape.setFillColor(sf::Color(128,0,0)); // Red to indicate dropping
            db.shape.setPosition(tableStartX+480, yPos);

            db.text.setFont(font);
            db.text.setString("Drop");
            db.text.setCharacterSize(20);
            db.text.setFillColor(sf::Color::White);
            db.text.setPosition(db.shape.getPosition().x + 16, db.shape.getPosition().y + 1);
            db.courseID = rec.courseID;

            dropButtons.push_back(db);
            yPos += 30;
        }
    };

    rebuildDropButtons(enrollmentRecords);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mp(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

                if (searchButton.getGlobalBounds().contains(mp)) {
                    // Show search page, don't close main window
                    showSearchPage();
                    // After returning from search page, refetch enrollments
                    enrollmentRecords = DatabaseManager::getEnrollmentsForStudent(currentStudent->getStudentID());
                    rebuildDropButtons(enrollmentRecords);
                }

                if (logoutButton.getGlobalBounds().contains(mp)) {
                    auth->logout(currentStudent->getUsername());
                    currentStudent = nullptr;
                    window.close();
                    std::cout << "User logged out.\n";
                }

                // Check if any drop button was clicked
                for (auto &db : dropButtons) {
                    if (db.shape.getGlobalBounds().contains(mp)) {
                        // Drop the course
                        if (enrollmentManager.dropEnrollment(currentStudent, db.courseID)) {
                            std::cout << "Dropped course ID: " << db.courseID << "\n";
                            // Refetch enrollments after dropping
                            enrollmentRecords = DatabaseManager::getEnrollmentsForStudent(currentStudent->getStudentID());
                            rebuildDropButtons(enrollmentRecords);
                            break; // break out after handling one button
                        } else {
                            std::cout << "Failed to drop course ID: " << db.courseID << "\n";
                        }
                    }
                }
            }
        }

        window.clear(sf::Color::White);
        // Draw main page elements
        window.draw(title);
        window.draw(line);
        window.draw(picSprite);
        window.draw(picOutline);
        window.draw(infoText);
        window.draw(searchButton);
        window.draw(searchText);
        window.draw(logoutButton);
        window.draw(logoutText);

        // Draw the enrollment table
        window.draw(tableTitle);
        window.draw(colCourseID);
        window.draw(colCourseName);
        window.draw(colEnrollmentDate);
        window.draw(colAction);
        window.draw(headerLine);

        float yPos = tableStartY + 90;
        for (size_t i = 0; i < enrollmentRecords.size(); ++i) {
            const auto& rec = enrollmentRecords[i];

            sf::Text cID(std::to_string(rec.courseID), font, 18);
            cID.setFillColor(sf::Color::Black);
            cID.setPosition(tableStartX, yPos);

            sf::Text cName(rec.courseName, font, 18);
            cName.setFillColor(sf::Color::Black);
            cName.setPosition(tableStartX+150, yPos);

            sf::Text cDate(rec.enrollmentDate, font, 18);
            cDate.setFillColor(sf::Color::Black);
            cDate.setPosition(tableStartX+300, yPos);

            window.draw(cID);
            window.draw(cName);
            window.draw(cDate);

            // Draw the drop button
            auto &db = dropButtons[i];
            window.draw(db.shape);
            window.draw(db.text);

            yPos += 30;
        }

        window.display();
    }
}

/**
 * Displays the search interface where students can search for courses by ID or name.
 * Allows students to enroll directly from the search results.
 */
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

    // Search button
    sf::RectangleShape doSearchButton(sf::Vector2f(120, 50));
    doSearchButton.setPosition(620, 180);
    doSearchButton.setFillColor(sf::Color(128,0,128));

    sf::Text doSearchButtonText("Search", font, 24);
    doSearchButtonText.setPosition(645, 190);
    doSearchButtonText.setFillColor(sf::Color::White);

    // Back button
    sf::RectangleShape backButton(sf::Vector2f(120, 50));
    backButton.setPosition(760, 180);
    backButton.setFillColor(sf::Color(128,0,128));

    sf::Text backButtonText("Back", font, 24);
    backButtonText.setPosition(795, 190);
    backButtonText.setFillColor(sf::Color::White);

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
            eb.shape.setFillColor(sf::Color(0, 128, 0));
            eb.shape.setPosition(900, yPos);
            eb.text.setFont(font);
            eb.text.setString("Enroll");
            eb.text.setCharacterSize(20);
            eb.text.setFillColor(sf::Color::White);
            eb.text.setPosition(eb.shape.getPosition().x + 13, eb.shape.getPosition().y + 2);
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
                return;
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

                if (backButton.getGlobalBounds().contains(mp)) {
                    // Close search window and return to the main menu without closing it
                    searchWindow.close();
                    return;
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

        // Draw back button
        searchWindow.draw(backButton);
        searchWindow.draw(backButtonText);

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
