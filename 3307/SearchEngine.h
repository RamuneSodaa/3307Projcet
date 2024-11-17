#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include "Course.h"
#include <vector>
#include <string>
#include <functional>

class SearchEngine {
public:
    // Search for courses by name or keywords
    std::vector<Course*> searchCourses(const std::vector<Course*>& courses, const std::string& keyword);

    // Filter courses by a specific condition (e.g., by capacity, prerequisites)
    std::vector<Course*> filterCourses(const std::vector<Course*>& courses, std::function<bool(Course*)> condition);

    // Sort courses by criteria (e.g., name, ID, capacity)
    std::vector<Course*> sortCourses(const std::vector<Course*>& courses, std::function<bool(Course*, Course*)> comparator);
};

#endif // SEARCHENGINE_H
