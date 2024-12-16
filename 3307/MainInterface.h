#ifndef MAININTERFACE_H
#define MAININTERFACE_H

#include "Student.h"
#include "Authentication.h"   // Added
#include "CourseManager.h"    // Added
#include "Scheduler.h"        // Added
#include <SFML/Graphics.hpp>

// Declare extern variables
extern Student* currentStudent;
extern Authentication* auth;
extern CourseManager courseManager;
extern Scheduler scheduler;

// Declare functions
bool showLoginPage();
bool showMainMenu();
void showMainPage();

void browseCourses();
void searchCourses();
void viewProfile();
void manageEnrollment();
void logout(sf::RenderWindow& window);

#endif
