#ifndef MAININTERFACE_H
#define MAININTERFACE_H
#include "Student.h"

// Declare the global variable
extern Student* currentStudent;

bool showMainMenu();
void login();
void browseCourses();
void searchCourses();
void viewProfile();
void manageEnrollment();
void logout();
#endif // MAININTERFACE_H
