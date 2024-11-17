#ifndef USER_H
#define USER_H

#include <string>

class User {
protected://
    int userID;
    std::string username;
    std::string password;

public:
    // Constructor and Destructor
    User(int id, const std::string& name, const std::string& pass);
    virtual ~User();

    // Getters
    int getUserID() const;
    std::string getUsername() const;

    // Authentication-related methods
    virtual bool login(const std::string& name, const std::string& pass);
    virtual void logout();

    // To verify user credentials (base functionality)
    virtual bool verifyCredentials(const std::string& name, const std::string& pass) const;
};

#endif // USER_H
