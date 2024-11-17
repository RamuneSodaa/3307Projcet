#include "Authentication.h"
#include <iostream>

// Initialize static member
Authentication* Authentication::instance = nullptr;

// Private constructor
Authentication::Authentication() {
    // Initialize some demo user credentials
    userCredentials["student1"] = "password123";
    userCredentials["student2"] = "securePass456";
    userCredentials["admin"] = "adminPass";

    // Initialize account state
    for (const auto& user : userCredentials) {
        failedAttempts[user.first] = 0;
        lockedAccounts[user.first] = false;
    }
}

// Get singleton instance
Authentication* Authentication::getInstance() {
    if (instance == nullptr) {
        instance = new Authentication();
    }
    return instance;
}

// Verify username and password
bool Authentication::verifyCredentials(const std::string& username, const std::string& password) {
    auto it = userCredentials.find(username);
    if (it == userCredentials.end()) {
        std::cout << "Username not found: " << username << std::endl;
        return false;
    }
    return it->second == password;
}

// Login function
bool Authentication::login(const std::string& username, const std::string& password) {
    if (isAccountLocked(username)) {
        std::cout << "Account locked for user: " << username << std::endl;
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

// Logout function
void Authentication::logout(const std::string& username) {
    std::cout << "User " << username << " has logged out." << std::endl;
}

// Add a new user
void Authentication::addUser(const std::string& username, const std::string& password) {
    userCredentials[username] = password;
    failedAttempts[username] = 0; // Reset failed attempts for the new user
    lockedAccounts[username] = false; // Ensure the account is not locked
    std::cout << "New user added: " << username << std::endl;
}

// Change password function
bool Authentication::changePassword(const std::string& username, const std::string& oldPassword, const std::string& newPassword) {
    if (verifyCredentials(username, oldPassword)) {
        userCredentials[username] = newPassword;
        std::cout << "Password changed successfully for user: " << username << std::endl;
        return true;
    }
    std::cout << "Password change failed for user: " << username << std::endl;
    return false;
}

// Increment failed login attempts
void Authentication::incrementFailedAttempts(const std::string& username) {
    failedAttempts[username]++;
    if (failedAttempts[username] >= 3) { // Lock account after 3 failed attempts
        lockedAccounts[username] = true;
        std::cout << "Account locked due to too many failed login attempts: " << username << std::endl;
    }
}

// Check if account is locked
bool Authentication::isAccountLocked(const std::string& username) {
    return lockedAccounts.find(username) != lockedAccounts.end() && lockedAccounts[username];
}

// Unlock account function
void Authentication::unlockAccount(const std::string& username) {
    if (lockedAccounts.find(username) != lockedAccounts.end()) {
        lockedAccounts[username] = false;
        failedAttempts[username] = 0;
        std::cout << "Account unlocked: .." << username << std::endl;
    } else {
        std::cout << "No account found for user: " << username << std::endl;
    }
}
