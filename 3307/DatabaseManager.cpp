#include "DatabaseManager.h"
#include <sqlite3.h>
#include <fstream>
#include <sstream>
#include <iostream>

static sqlite3* db = nullptr;

void DatabaseManager::initializeDatabase(const std::string& dbPath, const std::string& sqlPath) {
    char* errMsg = nullptr;

    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Since we now use IF NOT EXISTS, we can run init script once or multiple times safely
    // If tables already exist, they won't be recreated and data won't be lost.
    std::ifstream sqlFile(sqlPath);
    if (!sqlFile.is_open()) {
        std::cerr << "Could not open SQL initialization file: " << sqlPath << std::endl;
        return;
    }

    std::stringstream sqlStream;
    sqlStream << sqlFile.rdbuf();
    std::string sqlCommands = sqlStream.str();

    if (sqlite3_exec(db, sqlCommands.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "SQL error during database initialization: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "Database initialized successfully.\n";
    }

    printTables();
}

bool DatabaseManager::courseExists(int courseID) {
    std::ostringstream sql;
    sql << "SELECT COUNT(*) FROM courses WHERE course_id=" << courseID << ";";
    int count = 0;

    auto callback = [](void* data, int argc, char** argv, char**) -> int {
        int* cnt = (int*)data;
        if (argc > 0 && argv[0]) {
            *cnt = std::stoi(argv[0]);
        }
        return 0;
    };

    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql.str().c_str(), callback, &count, &errMsg) != SQLITE_OK) {
        std::cerr << "Error checking if course exists: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }

    return (count > 0);
}

bool DatabaseManager::studentExists(int studentID) {
    std::ostringstream sql;
    sql << "SELECT COUNT(*) FROM students WHERE student_id=" << studentID << ";";
    int count = 0;

    auto callback = [](void* data, int argc, char** argv, char**) -> int {
        int* cnt = (int*)data;
        if (argc > 0 && argv[0]) {
            *cnt = std::stoi(argv[0]);
        }
        return 0;
    };

    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql.str().c_str(), callback, &count, &errMsg) != SQLITE_OK) {
        std::cerr << "Error checking if student exists: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }

    return (count > 0);
}

void DatabaseManager::addCourse(const Course& course) {
    // Check if course already exists
    if (courseExists(course.getCourseID())) {
        std::cout << "Course with ID " << course.getCourseID() << " already exists. Skipping insert.\n";
        return;
    }

    std::ostringstream sql;
    sql << "INSERT INTO courses (course_id, name, schedule, capacity, prerequisites) VALUES ("
        << course.getCourseID() << ", '"
        << course.getCourseName() << "', '"
        << course.getSchedule() << "', "
        << course.getCapacity() << ", '"
        << course.getPrereqStr() << "');";
    executeQuery(sql.str());
}

void DatabaseManager::addStudent(const Student& student) {
    // Check if student already exists
    if (studentExists(student.getStudentID())) {
        std::cout << "Student with ID " << student.getStudentID() << " already exists. Skipping insert.\n";
        return;
    }

    std::ostringstream sql;
    sql << "INSERT INTO students (student_id, username, email, status) VALUES ("
        << student.getStudentID() << ", '"
        << student.getUsername() << "', '"
        << student.getEmail() << "', '"
        << student.getStatus() << "');";
    executeQuery(sql.str());
}

void DatabaseManager::addEnrollment(int studentID, int courseID, const std::string& courseName, const std::string& enrollmentDate) {
    std::ostringstream sql;
    sql << "INSERT INTO enrollments (student_id, course_id, course_name, enrollment_date) VALUES ("
        << studentID << ", "
        << courseID << ", '"
        << courseName << "', '"
        << enrollmentDate << "');";
    executeQuery(sql.str());
}

bool DatabaseManager::removeEnrollment(int studentID, int courseID) {
    std::ostringstream sql;
    sql << "DELETE FROM enrollments WHERE student_id=" << studentID
        << " AND course_id=" << courseID << ";";
    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql.str().c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "Failed to remove enrollment: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    std::cout << "Enrollment record removed: StudentID=" << studentID
              << ", CourseID=" << courseID << "\n";
    return true;
}

std::vector<Course> DatabaseManager::getAllCourses() {
    std::vector<Course> courses;
    const char* sql = "SELECT course_id, name, schedule, capacity, prerequisites FROM courses;";

    auto callback = [](void* data, int argc, char** argv, char**) -> int {
        std::vector<Course>* vec = static_cast<std::vector<Course>*>(data);
        if (argc == 5 && argv[0] && argv[1] && argv[2] && argv[3]) {
            int courseID = std::stoi(argv[0]);
            std::string name = argv[1];
            std::string schedule = argv[2];
            int capacity = std::stoi(argv[3]);
            std::string prereqStr = argv[4] ? argv[4] : "";
            vec->emplace_back(courseID, name, schedule, capacity, prereqStr);
        }
        return 0;
    };

    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql, callback, &courses, &errMsg) != SQLITE_OK) {
        std::cerr << "Error fetching courses: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }

    return courses;
}

std::vector<EnrollmentRecord> DatabaseManager::getEnrollmentsForStudent(int studentID) {
    std::vector<EnrollmentRecord> records;
    std::ostringstream sql;
    sql << "SELECT course_id, course_name, enrollment_date FROM enrollments WHERE student_id=" << studentID << ";";

    auto callback = [](void* data, int argc, char** argv, char**) -> int {
        std::vector<EnrollmentRecord>* recs = static_cast<std::vector<EnrollmentRecord>*>(data);
        if (argc == 3 && argv[0] && argv[1] && argv[2]) {
            EnrollmentRecord r;
            r.courseID = std::stoi(argv[0]);
            r.courseName = argv[1];
            r.enrollmentDate = argv[2];
            recs->push_back(r);
        }
        return 0;
    };

    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql.str().c_str(), callback, &records, &errMsg) != SQLITE_OK) {
        std::cerr << "Error fetching enrollments: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }

    return records;
}

void DatabaseManager::executeQuery(const std::string& query) {
    char* errMsg = nullptr;
    if (sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "SQL error executing query: " << query << "\n" << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "Query executed successfully: " << query << "\n";
    }
}

void DatabaseManager::printTables() {
    std::cout << "Checking created tables...\n";
    const char* tableQuery = "SELECT name FROM sqlite_master WHERE type='table';";
    char* errMsg = nullptr;

    auto callback = [](void*, int argc, char** argv, char** colName) -> int {
        for (int i = 0; i < argc; i++) {
            std::cout << colName[i] << ": " << (argv[i] ? argv[i] : "NULL") << "\n";
        }
        return 0;
    };

    if (sqlite3_exec(db, tableQuery, callback, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "Error retrieving tables: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    std::cout << "Finished checking tables.\n";
}
