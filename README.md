# **College Course Enrollment System**

## **Overview**
The College Course Enrollment System is a comprehensive C++ application with a GUI, designed to streamline course scheduling and management. The system provides functionality for students to enroll in courses and administrators to manage course information efficiently. It utilizes the **SFML (Simple and Fast Multimedia Library)** for GUI components and SQLite for database integration.

---

## **Features**
- **Student Features**:
  - Browse and search for available courses.
  - Enroll in courses, ensuring prerequisites are met.
  - View and manage personal course schedules.

- **Administrator Features**:
  - Add, update, and delete course information.
  - View and manage student enrollments.
  - Access real-time data through database integration.

- **Backend**:
  - Persistent data storage using SQLite.
  - Optimized scheduling algorithms to manage complex enrollments.

- **GUI**:
  - User-friendly graphical interfaces for students and administrators.
  - Multiple pages including:
    - Login Interface
    - Course Search Page
    - Main Dashboard
    - Administrator Panel

---

## **Technical Details**
- **Programming Language**: C++
- **GUI Framework**: SFML
- **Database**: SQLite
- **Design Patterns**:
  - Factory Pattern
  - Singleton Pattern
  - Observer Pattern
  - DAO (Data Access Object) Pattern

---

## **Project Structure**
```plaintext
3307/
├── Authentication.cpp/h       # User authentication logic
├── Course.cpp/h               # Course data and operations
├── CourseManager.cpp/h        # Manages course-related actions
├── DatabaseManager.cpp/h      # Handles SQLite database interactions
├── Scheduler.cpp/h            # Optimized course scheduling algorithms
├── MainMenuInterface.cpp/h    # GUI for the main menu
├── LoginInterface.cpp/h       # GUI for user login
├── PrerequisiteChecker.cpp/h  # Validates course prerequisites
├── assets/                    # Images and resources for the GUI
└── CMakeLists.txt             # Build configuration

Setup and Installation
Clone the Repository

Build the Project:
Ensure you have CMake and a C++ compiler installed.
Run the following commands:
mkdir build
cd build
cmake ..
make

Run the Application：
./CourseEnrollmentSystem
Database Setup:

Ensure SQLite is installed.
Initialize the database using the provided SQL scripts in assets/sql/.

Contributors
Shaotian Li
Xiaowei Feng