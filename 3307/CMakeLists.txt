cmake_minimum_required(VERSION 3.29)
project(3307)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 14)

# Include directories
include_directories(.)

# Add executable
add_executable(3307
        Main.cpp
        Authentication.cpp
        Course.cpp
        CourseManager.cpp
        EnrollmentStrategies.cpp
        PrerequisiteChecker.cpp
        Scheduler.cpp
        Student.cpp
        User.cpp
        Scheduler.cpp
)
