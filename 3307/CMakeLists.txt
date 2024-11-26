# Minimum required CMake version
cmake_minimum_required(VERSION 3.28)

# Set the project name
project(Project3307)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set SFML directory (assuming SFML folder is in the same directory as CMakeLists.txt)
set(SFML_DIR "${CMAKE_SOURCE_DIR}/SFML-2.6.2/lib/cmake/SFML")
message("SFML_DIR is set to: ${SFML_DIR}") # Print to confirm SFML path

# Set prefix path to make sure CMake searches for dependencies in the right location
set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/SFML-2.6.2")

# Include SFML headers manually
set(SFML_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/SFML-2.6.2/include")
include_directories(${SFML_INCLUDE_DIR})

# Set the library directory for SFML
set(SFML_LIB_DIR "${CMAKE_SOURCE_DIR}/SFML-2.6.2/lib")

# Set path to additional dependencies (freetype and openal)
link_directories("${SFML_LIB_DIR}")

# Handle different configurations for linking SFML
if (CMAKE_BUILD_TYPE MATCHES Debug)
    # Set SFML debug libraries
    set(SFML_LIBS
            "${SFML_LIB_DIR}/libsfml-graphics-d.a"
            "${SFML_LIB_DIR}/libsfml-window-d.a"
            "${SFML_LIB_DIR}/libsfml-system-d.a"
            "${SFML_LIB_DIR}/libfreetype.a"          # Link freetype library
            "${SFML_LIB_DIR}/libopenal32.a"          # Link openal library
    )
else()
    # Set SFML release libraries
    set(SFML_LIBS
            "${SFML_LIB_DIR}/libsfml-graphics.a"
            "${SFML_LIB_DIR}/libsfml-window.a"
            "${SFML_LIB_DIR}/libsfml-system.a"
            "${SFML_LIB_DIR}/libfreetype.a"          # Link freetype library
            "${SFML_LIB_DIR}/libopenal32.a"          # Link openal library
    )
endif()

# Add executable for the project
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
        SearchEngine.cpp
)

# Link SFML libraries and additional dependencies
target_link_libraries(${PROJECT_NAME}
        ${SFML_LIBS}
        winmm
        gdi32
)
