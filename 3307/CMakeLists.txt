# Minimum required CMake version
cmake_minimum_required(VERSION 3.28)

# Set the project name
project(Project3307)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories for project headers
include_directories(${CMAKE_SOURCE_DIR}/include)

# Set SFML directory (assuming SFML folder is in the same directory as CMakeLists.txt)
set(SFML_DIR "${CMAKE_SOURCE_DIR}/SFML-2.6.2/lib/cmake/SFML")
message("SFML_DIR is set to: ${SFML_DIR}") # Print to confirm SFML path

# Set prefix path for dependencies
set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/SFML-2.6.2")

# Include SFML headers manually
set(SFML_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/SFML-2.6.2/include")
include_directories(${SFML_INCLUDE_DIR})

# Include the directory where sqlite3.h is located
# Assuming sqlite3.c and sqlite3.h are in the same directory as CMakeLists.txt
include_directories(${CMAKE_SOURCE_DIR})

# Set the library directory for SFML
set(SFML_LIB_DIR "${CMAKE_SOURCE_DIR}/SFML-2.6.2/lib")
link_directories("${SFML_LIB_DIR}")

# Handle SFML libraries depending on build type
if (CMAKE_BUILD_TYPE MATCHES Debug)
    # SFML debug libraries
    set(SFML_LIBS
            "${SFML_LIB_DIR}/libsfml-graphics-d.a"
            "${SFML_LIB_DIR}/libsfml-window-d.a"
            "${SFML_LIB_DIR}/libsfml-system-d.a"
            "${SFML_LIB_DIR}/libfreetype.a"
            "${SFML_LIB_DIR}/libopenal32.a"
    )
else()
    # SFML release libraries
    set(SFML_LIBS
            "${SFML_LIB_DIR}/libsfml-graphics.a"
            "${SFML_LIB_DIR}/libsfml-window.a"
            "${SFML_LIB_DIR}/libsfml-system.a"
            "${SFML_LIB_DIR}/libfreetype.a"
            "${SFML_LIB_DIR}/libopenal32.a"
    )
endif()

# Set the Google Test directory and include it
set(GTEST_DIRECTORY "${CMAKE_SOURCE_DIR}/googletest-main")
add_subdirectory(${GTEST_DIRECTORY} ${CMAKE_BINARY_DIR}/googletest)

include_directories(${GTEST_DIRECTORY}/googletest/include)
include_directories(${GTEST_DIRECTORY}/googlemock/include)

# Define source files
set(SOURCE_FILES
        Authentication.cpp
        Course.cpp
        CourseManager.cpp
        PrerequisiteChecker.cpp
        Scheduler.cpp
        Student.cpp
        User.cpp
        MainMenuInterface.cpp
        sqlite3.c
        DatabaseManager.cpp
        LoginInterface.cpp
        Utils.h
        NotificationInterface.cpp
        EnrollmentManager.cpp
)

# Create a library from source files
add_library(core_lib STATIC ${SOURCE_FILES})

# Add executable for the project
add_executable(${PROJECT_NAME} Main.cpp)
target_link_libraries(${PROJECT_NAME} core_lib ${SFML_LIBS})

if(WIN32)
    target_link_libraries(${PROJECT_NAME} winmm gdi32)
endif()

# Add the test executable
add_executable(${PROJECT_NAME}_Tests
        tests/test_main.cpp
        tests/Authentication_test.cpp
        tests/CourseManager_test.cpp
)

# Link Google Test and Google Mock to the test executable
target_link_libraries(${PROJECT_NAME}_Tests
        gtest
        gtest_main
        gmock
        gmock_main
        core_lib  # Link with the library containing the main functionality
)

# Enable testing and discover tests
enable_testing()
include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_Tests)
