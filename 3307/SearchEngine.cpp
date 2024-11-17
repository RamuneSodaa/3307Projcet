#include "SearchEngine.h"
#include <algorithm>

// Search courses by name or keywords
std::vector<Course*> SearchEngine::searchCourses(const std::vector<Course*>& courses, const std::string& keyword) {
    std::vector<Course*> results;
    for (auto course : courses) {
        if (course->getCourseName().find(keyword) != std::string::npos) {
            results.push_back(course);
        }
    }
    return results;
}

// Filter courses based on a condition
std::vector<Course*> SearchEngine::filterCourses(const std::vector<Course*>& courses, std::function<bool(Course*)> condition) {
    std::vector<Course*> results;
    for (auto course : courses) {
        if (condition(course)) {
            results.push_back(course);
        }
    }
    return results;
}

// Sort courses based on a comparator
std::vector<Course*> SearchEngine::sortCourses(const std::vector<Course*>& courses, std::function<bool(Course*, Course*)> comparator) {
    std::vector<Course*> sortedCourses = courses; // Copy input vector
    std::sort(sortedCourses.begin(), sortedCourses.end(), comparator);
    return sortedCourses;
}
