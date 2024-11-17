#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include <string>
#include <unordered_map>

class Authentication {
private:
    static Authentication* instance; // Singleton instance
    std::unordered_map<std::string, std::string> userCredentials; // Stores username-password pairs

    // Private constructor
    Authentication();

public:
    // Prevent copying and assignment
    Authentication(const Authentication&) = delete;
    Authentication& operator=(const Authentication&) = delete;

    // Get the singleton instance
    static Authentication* getInstance();

    // Authentication functions
    bool verifyCredentials(const std::string& username, const std::string& password);
    bool login(const std::string& username, const std::string& password);
    void logout(const std::string& username);

    // Utility functions
    void addUser(const std::string& username, const std::string& password);
};

#endif // AUTHENTICATION_H
