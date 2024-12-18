#include "User.h"
#include <iostream>

// Constructor
User::User(int id, const std::string& name, const std::string& pass)
    : userID(id), username(name), password(pass) {}

// Destructor
User::~User() {}

// Getters
int User::getUserID() const {
    return userID;
}

std::string User::getUsername() const {
    return username;
}

// Login functionality
bool User::login(const std::string& name, const std::string& pass) {
    if (verifyCredentials(name, pass)) {
        std::cout << "User " << name << " logged in successfully." << std::endl;
        return true;
    }
    std::cout << "Login failed for user " << name << "." << std::endl;
    return false;
}

// Logout functionality
void User::logout() {
    std::cout << "User " << username << " has logged out." << std::endl;
}

// Verify credentials
bool User::verifyCredentials(const std::string& name, const std::string& pass) const {
    return (username == name && password == pass);
}
