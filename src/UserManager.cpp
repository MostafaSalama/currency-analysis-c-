#include "UserManager.h"
#include <fstream>
#include <iostream>
#include <algorithm>

UserManager::UserManager(std::string _csvFilename)
: csvFilename(_csvFilename)
{
    loadUsers();
}

bool UserManager::registerUser(std::string fullName, 
                               std::string email, 
                               std::string password, 
                               std::string& outUsername)
{
    // Check if user already exists
    if (isUserExists(email, fullName))
    {
        return false;
    }

    // Generate unique username
    std::string username;
    do {
        username = User::generateUsername();
    } while (isUsernameExists(username));

    // Hash the password
    size_t hashedPassword = User::hashPassword(password);

    // Create new user
    User newUser(username, fullName, email, hashedPassword);
    users.push_back(newUser);

    // Save to CSV
    saveUsers();

    // Return generated username
    outUsername = username;
    return true;
}

bool UserManager::isUserExists(std::string email, std::string fullName)
{
    for (const User& user : users)
    {
        if (user.getEmail() == email && user.getFullName() == fullName)
        {
            return true;
        }
    }
    return false;
}

bool UserManager::loginUser(std::string username, std::string password, User& outUser)
{
    // Hash the input password
    size_t hashedPassword = User::hashPassword(password);

    // Search for user with matching username and password
    for (const User& user : users)
    {
        if (user.getUsername() == username && 
            user.getHashedPassword() == hashedPassword)
        {
            outUser = user;
            return true;
        }
    }

    return false;
}

bool UserManager::verifyIdentity(std::string email, std::string fullName, User& outUser)
{
    for (const User& user : users)
    {
        if (user.getEmail() == email && user.getFullName() == fullName)
        {
            outUser = user;
            return true;
        }
    }
    return false;
}

bool UserManager::resetPassword(std::string username, std::string newPassword)
{
    // Hash the new password
    size_t hashedPassword = User::hashPassword(newPassword);

    // Find and update user
    for (size_t i = 0; i < users.size(); ++i)
    {
        if (users[i].getUsername() == username)
        {
            // Create new user with updated password
            users[i] = User(username, 
                          users[i].getFullName(), 
                          users[i].getEmail(), 
                          hashedPassword);
            
            // Save to CSV
            saveUsers();
            return true;
        }
    }

    return false;
}

void UserManager::loadUsers()
{
    users.clear();

    std::ifstream file(csvFilename);
    if (!file.is_open())
    {
        // File doesn't exist yet - this is OK for first run
        std::cout << "UserManager: Creating new user database." << std::endl;
        return;
    }

    std::string line;
    // Skip header line if it exists
    bool firstLine = true;
    while (std::getline(file, line))
    {
        if (firstLine && line.find("username") != std::string::npos)
        {
            firstLine = false;
            continue;
        }
        firstLine = false;

        if (line.empty())
            continue;

        try {
            User user = User::fromCSVLine(line);
            if (!user.getUsername().empty())
            {
                users.push_back(user);
            }
        }
        catch (const std::exception& e) {
            std::cout << "UserManager: Error parsing line: " << line << std::endl;
        }
    }

    file.close();
    std::cout << "UserManager: Loaded " << users.size() << " users." << std::endl;
}

void UserManager::saveUsers()
{
    std::ofstream file(csvFilename);
    if (!file.is_open())
    {
        std::cout << "UserManager: Error - Could not open file for writing: " 
                  << csvFilename << std::endl;
        return;
    }

    // Write header
    file << "username,fullName,email,hashedPassword" << std::endl;

    // Write all users
    for (const User& user : users)
    {
        file << user.toCSVLine() << std::endl;
    }

    file.close();
}

bool UserManager::isUsernameExists(std::string username)
{
    for (const User& user : users)
    {
        if (user.getUsername() == username)
        {
            return true;
        }
    }
    return false;
}
