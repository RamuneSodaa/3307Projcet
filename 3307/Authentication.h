#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include <string>
#include <unordered_map>


class Authentication {
private:
    static Authentication* instance; // Singleton instance
    std::unordered_map<std::string, std::string> userCredentials; // Stores username-password pairs
    std::unordered_map<std::string, int> failedAttempts; // Tracks failed login attempts for each user
    std::unordered_map<std::string, bool> lockedAccounts; // Tracks locked status of each account

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

    // Account management functions
    void addUser(const std::string& username, const std::string& password);

    // Security functions
    void incrementFailedAttempts(const std::string& username);
    bool isAccountLocked(const std::string& username);
    void unlockAccount(const std::string& username);
};

#endif // AUTHENTICATION_H
