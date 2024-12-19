#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <string>
#include <vector>
#include "Course.h"
#include "Student.h"

struct EnrollmentRecord {
    int courseID;
    std::string courseName;
    std::string enrollmentDate;
};

class DatabaseManager {
public:
    static void initializeDatabase(const std::string& dbPath, const std::string& sqlPath);
    static void addCourse(const Course& course);
    static void addStudent(const Student& student);
    static void addEnrollment(int studentID, int courseID, const std::string& courseName, const std::string& enrollmentDate);
    static bool removeEnrollment(int studentID, int courseID);
    static std::vector<Course> getAllCourses();
    static std::vector<EnrollmentRecord> getEnrollmentsForStudent(int studentID);

private:
    static void executeQuery(const std::string& query);
    static void printTables();

    // Helper methods to check existence
    static bool courseExists(int courseID);
    static bool studentExists(int studentID);
};

#endif
