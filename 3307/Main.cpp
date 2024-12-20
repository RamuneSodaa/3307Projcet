#include "Authentication.h"
#include "Student.h"
#include "DatabaseManager.h"
#include "CourseManager.h"
#include "Scheduler.h"
#include "PrerequisiteChecker.h"
#include "EnrollmentManager.h"
#include "LoginInterface.h"
#include "MainMenuInterface.h"
#include <iostream>

// Define extern variables once here
Student* currentStudent = nullptr;
Authentication* auth = Authentication::getInstance();

/**
 * Main function to initialize and run the application.
 *
 * Initializes the database, loads user data, manages course scheduling,
 * handles login and shows the main menu.
 *
 * @return Returns 0 upon successful execution and closure, and 1 upon failures.
 */
int main() {
    DatabaseManager::initializeDatabase("database.sqlite", "init_db.sql");

    CourseManager cm;
    Scheduler sched("Spring 2024");
    PrerequisiteChecker checker;
    EnrollmentManager em(cm, sched, checker);

    Student s1(12345678, "AliceW", "alicew@example.com", "active");
    Student s2(87654321, "BobC", "bobc@example.com", "active");
    Student s3(10000003, "CharlieM", "charliem@example.com", "active");
    Student s4(10000004, "DianaP", "dianap@example.com", "active");
    Student s5(10000005, "EthanH", "ethanh@example.com", "active");
    Student s6(10000006, "FionaK", "fionak@example.com", "active");
    Student s7(10000007, "GeorgeT", "georget@example.com", "active");
    Student s8(10000008, "HannahL", "hannahl@example.com", "active");
    Student s9(10000009, "IanR", "ianr@example.com", "active");
    Student s10(10000010, "JuliaV", "juliav@example.com", "active");
    Student s11(10000011, "KevinD", "kevind@example.com", "active");
    Student s12(10000012, "LilyS", "lilys@example.com", "active");
    Student s13(10000013, "MichaelB", "michaelb@example.com", "active");
    Student s14(10000014, "NoraF", "noraf@example.com", "active");
    Student s15(10000015, "OliverJ", "oliverj@example.com", "active");
    Student s16(10000016, "PaulaZ", "paulaz@example.com", "active");
    Student s17(10000017, "QuinnY", "quinny@example.com", "active");
    Student s18(10000018, "RachelX", "rachelx@example.com", "active");
    Student s19(10000019, "SamuelG", "samuelg@example.com", "active");
    Student s20(10000020, "TinaW", "tinaw@example.com", "active");

    // Add students to the database
    DatabaseManager::addStudent(s1);
    DatabaseManager::addStudent(s2);
    DatabaseManager::addStudent(s3);
    DatabaseManager::addStudent(s4);
    DatabaseManager::addStudent(s5);
    DatabaseManager::addStudent(s6);
    DatabaseManager::addStudent(s7);
    DatabaseManager::addStudent(s8);
    DatabaseManager::addStudent(s9);
    DatabaseManager::addStudent(s10);
    DatabaseManager::addStudent(s11);
    DatabaseManager::addStudent(s12);
    DatabaseManager::addStudent(s13);
    DatabaseManager::addStudent(s14);
    DatabaseManager::addStudent(s15);
    DatabaseManager::addStudent(s16);
    DatabaseManager::addStudent(s17);
    DatabaseManager::addStudent(s18);
    DatabaseManager::addStudent(s19);
    DatabaseManager::addStudent(s20);

    // Add user credentials for authentication
    auth->addUser("12345678", "12345678");
    auth->addUser("87654321", "87654321");
    auth->addUser("10000003", "My$3cureP@ss");
    auth->addUser("10000004", "D!anaP@ss456");
    auth->addUser("10000005", "Eth@nSecure#789");
    auth->addUser("10000006", "F!0naP@ssword");
    auth->addUser("10000007", "Ge0rge#Pass2023");
    auth->addUser("10000008", "H@nnah123$");
    auth->addUser("10000009", "Ian_P@ss!456");
    auth->addUser("10000010", "Juli@P@ss987");
    auth->addUser("10000011", "Kev!nPass#456");
    auth->addUser("10000012", "LilY#Secure!");
    auth->addUser("10000013", "Mich@el$123");
    auth->addUser("10000014", "Nor@P@ssword2023");
    auth->addUser("10000015", "0liver#P@ss456");
    auth->addUser("10000016", "P@ul@Secur3");
    auth->addUser("10000017", "Qu!nnP@ssword#");
    auth->addUser("10000018", "Rach3l_Secur3$");
    auth->addUser("10000019", "S@muelP@ss123");
    auth->addUser("10000020", "T!naP@ss456$");


    Course c1(101, "Math I", "Monday 8:00-9:00", 40);
    Course c2(201, "Math II", "Monday 11:00-12:00", 40, "101");
    Course c3(102, "Biology I", "Monday 14:00-15:00", 40);
    Course c4(202, "Biology II", "Monday 10:00-11:00", 40, "102");
    DatabaseManager::addCourse(c1);
    DatabaseManager::addCourse(c2);
    DatabaseManager::addCourse(c3);
    DatabaseManager::addCourse(c4);

    cm.loadCoursesFromDatabase();

    // Resolve prerequisites for all courses before enrollment checks
    for (auto c : cm.getCourseList()) {
        c->resolvePrerequisites(cm.getCourseList());
    }

    for (auto c : cm.getCourseList()) {
        sched.addCourse(c);
    }

    LoginInterface login;
    if (login.show()) {
        std::cout << "Login successful. Welcome: " << currentStudent->getUsername() << "\n";

        MainMenuInterface mainMenu(cm, sched, em);
        mainMenu.show();
    } else {
        std::cout << "Login failed or user exited.\n";
    }

    return 0;
}
