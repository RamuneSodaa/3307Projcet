cmake_minimum_required(VERSION 3.28)
project(3307)

set(CMAKE_CXX_STANDARD 14)

# Set the wxWidgets installation path
set(wxWidgets_ROOT_DIR "C:/wxWidgets/wxWidgets-3.1.5")
set(wxWidgets_LIB_DIR "${wxWidgets_ROOT_DIR}/lib/gcc_lib")

# Add wxWidgets include directories
include_directories("${wxWidgets_ROOT_DIR}/include")
include_directories("${wxWidgets_LIB_DIR}/mswu") # Adjust this path depending on your build configuration

# Add source include directories
include_directories(.)

# Add the executable and sources
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
        PrerequisiteChecker.cpp
)

# Link against wxWidgets static libraries
target_link_libraries(3307
        "-lwxbase31u"
        "-lwxmsw31u_core"
        "-lwxmsw31u_adv"
        "-lgdi32"
        "-lcomdlg32"
        "-lole32"
        "-loleaut32"
        "-luuid"
        "-lwinmm"
        "-lshell32"
)
