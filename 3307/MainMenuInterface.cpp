#include "MainMenuInterface.h"
#include "Student.h"
#include "Authentication.h"
#include "CourseManager.h"
#include "Scheduler.h"
#include "NotificationInterface.h"
#include "PrerequisiteChecker.h"
#include <SFML/Graphics.hpp>
#include <algorithm>

extern Student* currentStudent;
extern Authentication* auth;
extern CourseManager courseManager;
extern Scheduler scheduler;

enum PageState {
    MAIN_PAGE,
    SEARCH_PAGE
};

void MainMenuInterface::show() {
    if(!currentStudent) return;

    sf::RenderWindow window(sf::VideoMode(1280,720),"Student Center - Main Page");
    {
        sf::Image icon;
        icon.loadFromFile("../assets/icon.png");
        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    }

    sf::Font font; font.loadFromFile("../assets/fonts/OpenSans-Regular.ttf");

    sf::Text title("Main Menu",font,40);
    title.setPosition(100,50);
    title.setFillColor(sf::Color::Black);

    sf::RectangleShape line(sf::Vector2f(900,2));
    line.setPosition(100,100);
    line.setFillColor(sf::Color::Black);

    sf::Texture picTex;
    picTex.loadFromFile("../assets/user_profile.png");
    sf::Sprite picSprite(picTex);
    picSprite.setPosition(130,120);
    picSprite.setScale(0.13f,0.13f);
    sf::FloatRect picBounds = picSprite.getGlobalBounds();
    sf::RectangleShape picOutline(sf::Vector2f(picBounds.width+8, picBounds.height+8));
    picOutline.setPosition(picBounds.left-4, picBounds.top-4);
    picOutline.setFillColor(sf::Color::Transparent);
    picOutline.setOutlineColor(sf::Color::Black);
    picOutline.setOutlineThickness(5);

    sf::Text infoText("",font,24);
    infoText.setString("ID: "+std::to_string(currentStudent->getStudentID())+"\nName: "+currentStudent->getUsername()+"\nEmail: "+currentStudent->getEmail());
    infoText.setPosition(100,290);
    infoText.setFillColor(sf::Color::Black);

    sf::RectangleShape searchButton(sf::Vector2f(200,50));
    searchButton.setPosition(100,430);
    searchButton.setFillColor(sf::Color(128,0,128));
    sf::Text searchText("Search Courses",font,24);
    searchText.setPosition(120,440);
    searchText.setFillColor(sf::Color::White);

    sf::RectangleShape logoutButton(sf::Vector2f(200,50));
    logoutButton.setPosition(100,500);
    logoutButton.setFillColor(sf::Color(128,0,128));
    sf::Text logoutText("Logout",font,24);
    logoutText.setPosition(160,510);
    logoutText.setFillColor(sf::Color::White);

    PageState currentPage = MAIN_PAGE;

    bool focusOnQuery=false;
    std::string queryStr;
    bool cursorVisible=true;
    sf::Clock cursorClock;
    sf::RectangleShape cursor(sf::Vector2f(1,20));
    cursor.setFillColor(sf::Color::Black);
    bool searchPerformed=false;
    std::vector<Course*> results;
    bool singleCourseFound=false;
    Course* singleCourse=nullptr;

    sf::Text searchPageTitle("Search Courses",font,30);
    searchPageTitle.setPosition(100,50);
    searchPageTitle.setFillColor(sf::Color::Black);

    sf::Text label("ID/partial name:",font,20);
    label.setPosition(100,90);
    label.setFillColor(sf::Color::Black);

    sf::RectangleShape queryBg(sf::Vector2f(200,30));
    queryBg.setPosition(100,115);
    queryBg.setFillColor(sf::Color(230,230,250));
    queryBg.setOutlineColor(sf::Color::Black);
    queryBg.setOutlineThickness(1);

    sf::Text queryInput("",font,20);
    queryInput.setPosition(105,118);
    queryInput.setFillColor(sf::Color::Black);

    sf::RectangleShape searchBtn(sf::Vector2f(100,30));
    searchBtn.setPosition(320,115);
    searchBtn.setFillColor(sf::Color(128,0,128));

    sf::Text searchBtnText("Search",font,20);
    searchBtnText.setPosition(338,117);
    searchBtnText.setFillColor(sf::Color::White);

    sf::RectangleShape backBtn(sf::Vector2f(110,40));
    backBtn.setPosition(100,500);
    backBtn.setFillColor(sf::Color(128,0,128));
    sf::Text backText("BACK",font,25);
    backText.setPosition(122,505);
    backText.setFillColor(sf::Color::White);

    sf::Text resultText("",font,20);
    resultText.setFillColor(sf::Color::Black);

    sf::RectangleShape enrollBtn(sf::Vector2f(120,40));
    enrollBtn.setFillColor(sf::Color(0,128,0));
    sf::Text enrollTxt("Enroll",font,30);
    enrollTxt.setFillColor(sf::Color::White);

    float tableX=500;
    float tableY=150;
    float tableW=500;
    float tableH=400;

    // No outer border: do not draw tableBox or set outline thickness to 0
    // If you want no outer frame at all, don't draw tableBox:
    // sf::RectangleShape tableBox(sf::Vector2f(tableW,tableH));
    // tableBox.setPosition(tableX,tableY);
    // tableBox.setFillColor(sf::Color(245,245,245));
    // tableBox.setOutlineThickness(0); // 0 outline to remove border
    // do not window.draw(tableBox);

    sf::Text colHeaders("",font,20);
    colHeaders.setString("ID    Name                Schedule           Capacity");
    // Move headers down a bit more, e.g., +30 from tableY for spacing
    colHeaders.setPosition(tableX, tableY);
    colHeaders.setFillColor(sf::Color::Black);

    float colID = tableX+50;
    float colName = tableX+200;
    float colSchedule = tableX+400;
    float colCapacity = tableX+600;

    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                window.close();
            }

            if(currentPage == MAIN_PAGE) {
                if(event.type == sf::Event::MouseButtonPressed) {
                    sf::Vector2f mp=(sf::Vector2f)sf::Mouse::getPosition(window);
                    if(searchButton.getGlobalBounds().contains(mp)) {
                        currentPage=SEARCH_PAGE;
                        queryStr.clear();
                        queryInput.setString("");
                        searchPerformed=false;singleCourseFound=false;singleCourse=nullptr;results.clear();
                    } else if(logoutButton.getGlobalBounds().contains(mp)) {
                        delete currentStudent;currentStudent=nullptr;window.close();
                    }
                }
            } else if(currentPage == SEARCH_PAGE) {
                if(event.type==sf::Event::TextEntered && focusOnQuery && event.text.unicode<128) {
                    if(event.text.unicode=='\b') {
                        if(!queryStr.empty()) {queryStr.pop_back();queryInput.setString(queryStr);}
                    } else {
                        char c=(char)event.text.unicode;
                        if(!std::iscntrl((unsigned char)c) && queryStr.size()<20) {
                            queryStr+=c;queryInput.setString(queryStr);
                        }
                    }
                }
                if(event.type==sf::Event::MouseButtonPressed) {
                    sf::Vector2f mp=(sf::Vector2f)sf::Mouse::getPosition(window);
                    if(queryBg.getGlobalBounds().contains(mp)) focusOnQuery=true; else focusOnQuery=false;
                    if(searchBtn.getGlobalBounds().contains(mp)) {
                        if(!queryStr.empty()) {
                            std::string lowerQ=queryStr;
                            std::transform(lowerQ.begin(),lowerQ.end(),lowerQ.begin(),::tolower);
                            results=courseManager.searchCourses(lowerQ);
                            searchPerformed=true; singleCourseFound=false; singleCourse=nullptr;
                            if(results.empty()) {
                                resultText.setString("No course found.");
                            } else if(results.size()==1) {
                                singleCourseFound=true; singleCourse=results.front();
                                resultText.setString("Found:\nID: "+std::to_string(singleCourse->getCourseID())+
                                                     "\nName: "+singleCourse->getCourseName()+
                                                     "\nSchedule: "+singleCourse->getSchedule()+
                                                     "\nCapacity: "+std::to_string(singleCourse->getCapacity()));
                            } else {
                                std::string rs;
                                for(auto c: results) {
                                    rs+="ID: "+std::to_string(c->getCourseID())+
                                        "\nName: "+c->getCourseName()+
                                        "\nSchedule: "+c->getSchedule()+
                                        "\nCapacity: "+std::to_string(c->getCapacity())+
                                        "\n----------\n";
                                }
                                resultText.setString(rs);
                            }
                        }
                    } else if(backBtn.getGlobalBounds().contains(mp)) {
                        currentPage=MAIN_PAGE;
                    } else if(singleCourseFound && singleCourse && results.size()==1 && enrollBtn.getGlobalBounds().contains(mp)) {
                        if(scheduler.scheduleCourse(currentStudent, singleCourse->getCourseID())) {
                            resultText.setString(resultText.getString()+"\nEnrollment successful!");
                        } else {
                            NotificationInterface().show("Enrollment failed. Possibly prerequisites not met or course full.");
                        }
                    }
                }
            }
        }

        if(cursorClock.getElapsedTime().asMilliseconds()>500) {
            cursorVisible=!cursorVisible;
            cursorClock.restart();
        }

        window.clear(sf::Color::White);

        if(currentPage == MAIN_PAGE) {
            window.draw(title);
            window.draw(line);
            window.draw(picSprite);
            window.draw(picOutline);
            window.draw(infoText);
            window.draw(searchButton);
            window.draw(searchText);
            window.draw(logoutButton);
            window.draw(logoutText);

            // Draw column headers only (no outer frame)
            window.draw(colHeaders);

            // Draw vertical dividing lines
            sf::RectangleShape vLine(sf::Vector2f(2,tableH-20));
            vLine.setFillColor(sf::Color::Black);

            // Align vertical lines with headers
            vLine.setPosition(colID,tableY+40); window.draw(vLine);
            vLine.setPosition(colName,tableY+40); window.draw(vLine);
            vLine.setPosition(colSchedule,tableY+40); window.draw(vLine);
            vLine.setPosition(colCapacity,tableY+40); window.draw(vLine);

            bool empty = currentStudent->getRegisteredCourses().empty();
            int yPos=(int)(tableY+80); // Shift down for rows

            if(empty) {
                sf::Text noCourses("No enrolled courses found.",font,20);
                noCourses.setPosition(tableX+10,tableY+80);
                noCourses.setFillColor(sf::Color::Black);
                window.draw(noCourses);
            } else {
                for(auto c: currentStudent->getRegisteredCourses()) {
                    sf::Text idT(std::to_string(c->getCourseID()),font,20);
                    idT.setFillColor(sf::Color::Black);
                    idT.setPosition(tableX+10,(float)yPos);

                    sf::Text nameT(c->getCourseName(),font,20);
                    nameT.setFillColor(sf::Color::Black);
                    nameT.setPosition(colID+10,(float)yPos);

                    sf::Text schedT(c->getSchedule(),font,20);
                    schedT.setFillColor(sf::Color::Black);
                    schedT.setPosition(colName+10,(float)yPos);

                    sf::Text capT(std::to_string(c->getCapacity()),font,20);
                    capT.setFillColor(sf::Color::Black);
                    capT.setPosition(colSchedule+10,(float)yPos);

                    window.draw(idT);
                    window.draw(nameT);
                    window.draw(schedT);
                    window.draw(capT);

                    sf::RectangleShape hLine(sf::Vector2f(tableW-2,2));
                    hLine.setFillColor(sf::Color::Black);
                    hLine.setPosition(tableX+1,(float)(yPos+28));
                    window.draw(hLine);

                    yPos+=30;
                }
            }

        } else if(currentPage == SEARCH_PAGE) {
            window.draw(searchPageTitle);
            window.draw(label);
            window.draw(queryBg);
            window.draw(queryInput);
            window.draw(searchBtn);
            window.draw(searchBtnText);
            window.draw(backBtn);
            window.draw(backText);
            if(focusOnQuery && cursorVisible) {
                cursor.setPosition(queryInput.getPosition().x+queryInput.getLocalBounds().width+5,queryInput.getPosition().y+5);
                window.draw(cursor);
            }
            if(searchPerformed) {
                if(!results.empty()) {
                    float rx=100; float ry=160; float rw=300; float rh=200;
                    sf::RectangleShape box(sf::Vector2f(rw,rh));
                    box.setPosition(rx,ry);
                    box.setFillColor(sf::Color(245,245,245));
                    box.setOutlineColor(sf::Color::Black);
                    box.setOutlineThickness(1);
                    window.draw(box);
                    resultText.setPosition(rx+10,ry+10);
                    window.draw(resultText);
                    if(singleCourseFound && singleCourse && results.size()==1) {
                        enrollBtn.setPosition(rx+10,ry+120);
                        enrollTxt.setPosition(rx+20,ry+125);
                        window.draw(enrollBtn);
                        window.draw(enrollTxt);
                    }
                } else {
                    resultText.setPosition(100,160);
                    window.draw(resultText);
                }
            }
        }

        window.display();
    }
}
