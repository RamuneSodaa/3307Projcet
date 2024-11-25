cmake_minimum_required(VERSION 3.28)

# Set the project name
project(Project3307)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
add_executable(${PROJECT_NAME}
        Main.cpp
        Authentication.cpp
        Course.cpp
        CourseManager.cpp
        EnrollmentStrategies.cpp
        PrerequisiteChecker.cpp
        Scheduler.cpp
        Student.cpp
        User.cpp
)
