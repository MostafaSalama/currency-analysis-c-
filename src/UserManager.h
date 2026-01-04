#pragma once

#include <string>
#include <vector>
#include "User.h"

class UserManager
{
public:
    /** Constructor - loads users from CSV file */
    UserManager(std::string csvFilename = "users.csv");

    /** 
     * Register a new user
     * @param fullName User's full name
     * @param email User's email address
     * @param password Plain text password (will be hashed)
     * @param outUsername Reference to store generated username
     * @return true if registration successful, false if user already exists
     */
    bool registerUser(std::string fullName, 
                      std::string email, 
                      std::string password, 
                      std::string& outUsername);

    /**
     * Check if user with email and full name already exists
     * @param email User's email
     * @param fullName User's full name
     * @return true if user exists, false otherwise
     */
    bool isUserExists(std::string email, std::string fullName);

    /**
     * Login user with username and password
     * @param username 10-digit username
     * @param password Plain text password
     * @param outUser Reference to store user object if login successful
     * @return true if login successful, false otherwise
     */
    bool loginUser(std::string username, std::string password, User& outUser);

    /**
     * Verify user identity using email and full name
     * @param email User's email
     * @param fullName User's full name
     * @param outUser Reference to store user object if found
     * @return true if identity verified, false otherwise
     */
    bool verifyIdentity(std::string email, std::string fullName, User& outUser);

    /**
     * Reset user password
     * @param username User's username
     * @param newPassword New plain text password (will be hashed)
     * @return true if password reset successful, false otherwise
     */
    bool resetPassword(std::string username, std::string newPassword);

private:
    std::string csvFilename;
    std::vector<User> users;

    /** Load users from CSV file */
    void loadUsers();

    /** Save users to CSV file */
    void saveUsers();

    /** Check if username already exists (for uniqueness) */
    bool isUsernameExists(std::string username);
};
