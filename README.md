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

## **Icon used**

![icon.png](3307%2Fassets%2Ficon.png)

![user_profile.png](3307%2Fassets%2Fuser_profile.png)

All images used in the project created by ChatGPT: https://chatgpt.com/share/6756e190-0414-8006-8cb6-5458b7d35882

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
Main.cpp

Database reminder:
Database should have be created, if not run the SQL file in cmake-build-debug

Contributors
Shaotian Li
Xiaowei Feng





