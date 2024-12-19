#ifndef MAINMENUINTERFACE_H
#define MAINMENUINTERFACE_H

#include <SFML/Graphics.hpp>
#include "CourseManager.h"
#include "Scheduler.h"
#include "EnrollmentManager.h"

class MainMenuInterface {
public:
    MainMenuInterface(CourseManager& cm, Scheduler& sched, EnrollmentManager& em);
    void show();

private:
    CourseManager& courseManager;
    Scheduler& scheduler;
    EnrollmentManager& enrollmentManager;

    sf::RenderWindow window;
    sf::Font font;

    void showSearchPage();
};

#endif // MAINMENUINTERFACE_H
