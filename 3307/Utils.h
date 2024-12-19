#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <algorithm>
#include <cctype>

inline std::string trim(const std::string& str) {
    // Find the first character that is not a whitespace
    auto start = str.begin();
    while (start != str.end() && std::isspace((unsigned char)*start)) {
        start++;
    }

    // Find the last character that is not a whitespace
    auto end = str.end();
    while (end != start && std::isspace((unsigned char)*(end - 1))) {
        end--;
    }

    return std::string(start, end);
}

#endif
