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
    return (it != userCredentials.end() && it->second == password);
}

// Login function
bool Authentication::login(const std::string& username, const std::string& password) {
    if (verifyCredentials(username, password)) {
        std::cout << "Login successful for user: " << username << std::endl;
        return true;
    }
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
    std::cout << "New user added: " << username << std::endl;
}
