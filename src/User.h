#pragma once

#include <string>

class User
{
public:
    /** Default constructor for empty/invalid user */
    User();
    
    /** Constructor to create a user object */
    User(std::string username, 
         std::string fullName, 
         std::string email, 
         size_t hashedPassword);

    /** Get the username (10-digit ID) */
    std::string getUsername() const;

    /** Get the user's full name */
    std::string getFullName() const;

    /** Get the user's email address */
    std::string getEmail() const;

    /** Get the hashed password */
    size_t getHashedPassword() const;

    /** Hash a password using std::hash */
    static size_t hashPassword(const std::string& password);

    /** Generate a unique 10-digit username */
    static std::string generateUsername();

    /** Convert user to CSV line format */
    std::string toCSVLine() const;

    /** Create user from CSV line */
    static User fromCSVLine(const std::string& line);

private:
    std::string username;       // 10-digit unique ID
    std::string fullName;       // User's full name
    std::string email;          // User's email address
    size_t hashedPassword;      // Hashed password value
};
