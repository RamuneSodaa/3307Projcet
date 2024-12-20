#include "Authentication.h"
#include "NotificationInterface.h"
#include <iostream>
#include <algorithm>

Authentication* Authentication::instance = nullptr;
Authentication::Authentication() {
    for (const auto& user : userCredentials) {
        failedAttempts[user.first] = 0;
        lockedAccounts[user.first] = false;
    }
}

/**
 * Static method to get the singleton instance of the Authentication class.
 * If the instance does not exist, it is created. Otherwise, the existing instance is returned.
 *
 * @return Pointer to the singleton Authentication instance.
 */
Authentication* Authentication::getInstance() {
    if (instance == nullptr) {
        instance = new Authentication();
    }
    return instance;
}

/**
 * Verifies if the provided username and password match those stored in the system.
 *
 * @param username The username of the user trying to log in.
 * @param password The password provided by the user.
 * @return true if the credentials match, false otherwise.
 */
bool Authentication::verifyCredentials(const std::string& username, const std::string& password) {
    auto it = userCredentials.find(username);
    if (it == userCredentials.end()) {
        NotificationInterface::show("Username not found: " + username) ;
        return false;
    }
    return it->second == password;
}

/**
 * Attempts to log a user in with the provided username and password.
 * If the account is locked or the credentials do not match, the login will fail.
 *
 * @param username The username of the user trying to log in.
 * @param password The password provided by the user.
 * @return true if login is successful, false otherwise.
 */
bool Authentication::login(const std::string& username, const std::string& password) {
    if (isAccountLocked(username)) {
        NotificationInterface::show("Account locked for user: " + username) ;
        return false;
    }
    if (verifyCredentials(username, password)) {
        std::cout << "Login successful for user: " << username << std::endl;
        return true;
    }
    incrementFailedAttempts(username);
    std::cout << "Login failed for user: " << username << std::endl;
    return false;
}

/**
 * Logs a user out of the system.
 *
 * @param username The username of the user logging out.
 */
void Authentication::logout(const std::string& username) {
    std::cout << "User " << username << " has logged out." << std::endl;
}

/**
 * Adds a new user to the system with the specified username and password.
 *
 * @param username The username for the new user.
 * @param password The password for the new user.
 */
void Authentication::addUser(const std::string& username, const std::string& password) {
    userCredentials[username] = password;
    failedAttempts[username] = 0; // Reset failed attempts for the new user
    lockedAccounts[username] = false; // Ensure the account is not locked
    std::cout << "New user added: " << username << std::endl;
}

/**
 * Increments the failed login attempts for a user and locks the account if the maximum attempts are exceeded.
 *
 * @param username The username of the user whose failed attempts are to be incremented.
 */
void Authentication::incrementFailedAttempts(const std::string& username) {
    failedAttempts[username]++;
    if (failedAttempts[username] >= 3) { // Lock account after 3 failed attempts
        lockedAccounts[username] = true;
        std::cout << "Account locked due to too many failed login attempts: " << username << std::endl;
    }
}

/**
 * Checks if a user's account is locked.
 *
 * @param username The username of the user to check.
 * @return true if the account is locked, false otherwise.
 */
bool Authentication::isAccountLocked(const std::string& username) {
    return lockedAccounts.find(username) != lockedAccounts.end() && lockedAccounts[username];
}

/**
 * Unlocks a user's account and resets their failed login attempts.
 *
 * @param username The username of the user whose account is to be unlocked.
 */
void Authentication::unlockAccount(const std::string& username) {
    if (lockedAccounts.find(username) != lockedAccounts.end()) {
        lockedAccounts[username] = false;
        failedAttempts[username] = 0;
        std::cout << "Account unlocked: .." << username << std::endl;
    } else {
        std::cout << "No account found for user: " << username << std::endl;
    }
}
