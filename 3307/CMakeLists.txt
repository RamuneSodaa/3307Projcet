cmake_minimum_required(VERSION 3.28)
project(3307)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(3307
    Authentication.cpp
    Authentication.h
    Course.cpp
    Course.h
    CourseManager.cpp
    CourseManager.h
    EnrollmentStrategies.cpp
    EnrollmentStrategies.h
    Main.cpp
    Scheduler.cpp
    Scheduler.h
    User.cpp
    User.h
        Student.cpp
        PrerequisiteChecker.h
        PrerequisiteChecker.cpp)
