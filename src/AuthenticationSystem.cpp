#include "AuthenticationSystem.h"
#include <iostream>
#include <limits>

AuthenticationSystem::AuthenticationSystem()
: userManager("users.csv")
{
}

User AuthenticationSystem::authenticate()
{
    while (true)
    {
        printAuthMenu();
        int choice = getAuthOption();

        switch (choice)
        {
            case 1: // Login
            {
                User user = handleLogin();
                if (!user.getUsername().empty())
                {
                    return user; // Successful login
                }
                break;
            }
            case 2: // Register
            {
                User user = handleRegister();
                if (!user.getUsername().empty())
                {
                    return user; // Successful registration and auto-login
                }
                break;
            }
            case 3: // Forgot Password
                handlePasswordReset();
                break;
            case 4: // Exit
                std::cout << "Goodbye!" << std::endl;
                return User(); // Return empty user
            default:
                std::cout << "Invalid choice. Please select 1-4." << std::endl;
                break;
        }
    }
}

void AuthenticationSystem::printAuthMenu()
{
    std::cout << std::endl;
    std::cout << "===================================" << std::endl;
    std::cout << "=== MerkelRex Trading Platform ===" << std::endl;
    std::cout << "===================================" << std::endl;
    std::cout << "1: Login" << std::endl;
    std::cout << "2: Register New Account" << std::endl;
    std::cout << "3: Forgot Password" << std::endl;
    std::cout << "4: Exit" << std::endl;
    std::cout << "===================================" << std::endl;
}

int AuthenticationSystem::getAuthOption()
{
    int option = 0;
    std::string line;
    std::cout << "Enter your choice (1-4): ";
    std::getline(std::cin, line);
    
    try {
        option = std::stoi(line);
    }
    catch (const std::exception& e) {
        return 0; // Invalid input
    }
    
    return option;
}

User AuthenticationSystem::handleLogin()
{
    std::cout << std::endl;
    std::cout << "=== Login ===" << std::endl;
    
    std::string username;
    std::cout << "Enter username: ";
    std::getline(std::cin, username);
    
    std::string password;
    std::cout << "Enter password: ";
    std::getline(std::cin, password);
    
    User user;
    if (userManager.loginUser(username, password, user))
    {
        std::cout << std::endl;
        std::cout << "Login successful! Welcome, " << user.getFullName() << "!" << std::endl;
        return user;
    }
    else
    {
        std::cout << std::endl;
        std::cout << "Login failed. Invalid username or password." << std::endl;
        return User(); // Return empty user
    }
}

User AuthenticationSystem::handleRegister()
{
    std::cout << std::endl;
    std::cout << "=== Register New Account ===" << std::endl;
    
    std::string fullName;
    std::cout << "Enter your full name: ";
    std::getline(std::cin, fullName);
    
    if (fullName.empty())
    {
        std::cout << "Error: Full name cannot be empty." << std::endl;
        return User();
    }
    
    std::string email;
    std::cout << "Enter your email address: ";
    std::getline(std::cin, email);
    
    if (email.empty())
    {
        std::cout << "Error: Email cannot be empty." << std::endl;
        return User();
    }
    
    // Check for duplicate user
    if (userManager.isUserExists(email, fullName))
    {
        std::cout << std::endl;
        std::cout << "Error: A user with this email and name already exists." << std::endl;
        std::cout << "Please use the login option or try password reset." << std::endl;
        return User();
    }
    
    std::string password;
    std::cout << "Enter a password: ";
    std::getline(std::cin, password);
    
    if (password.empty())
    {
        std::cout << "Error: Password cannot be empty." << std::endl;
        return User();
    }
    
    std::string confirmPassword;
    std::cout << "Confirm password: ";
    std::getline(std::cin, confirmPassword);
    
    if (password != confirmPassword)
    {
        std::cout << std::endl;
        std::cout << "Error: Passwords do not match." << std::endl;
        return User();
    }
    
    // Register the user
    std::string username;
    if (userManager.registerUser(fullName, email, password, username))
    {
        std::cout << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "Registration successful!" << std::endl;
        std::cout << "Your username is: " << username << std::endl;
        std::cout << "Please save this username for future logins." << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << std::endl;
        std::cout << "Logging you in automatically..." << std::endl;
        
        // Auto-login the user
        User user;
        userManager.loginUser(username, password, user);
        return user;
    }
    else
    {
        std::cout << std::endl;
        std::cout << "Error: Registration failed." << std::endl;
        return User();
    }
}

void AuthenticationSystem::handlePasswordReset()
{
    std::cout << std::endl;
    std::cout << "=== Password Reset ===" << std::endl;
    
    std::string email;
    std::cout << "Enter your email address: ";
    std::getline(std::cin, email);
    
    std::string fullName;
    std::cout << "Enter your full name: ";
    std::getline(std::cin, fullName);
    
    User user;
    if (userManager.verifyIdentity(email, fullName, user))
    {
        std::cout << std::endl;
        std::cout << "Identity verified!" << std::endl;
        std::cout << "Your username is: " << user.getUsername() << std::endl;
        std::cout << std::endl;
        
        std::string newPassword;
        std::cout << "Enter your new password: ";
        std::getline(std::cin, newPassword);
        
        if (newPassword.empty())
        {
            std::cout << "Error: Password cannot be empty." << std::endl;
            return;
        }
        
        std::string confirmPassword;
        std::cout << "Confirm new password: ";
        std::getline(std::cin, confirmPassword);
        
        if (newPassword != confirmPassword)
        {
            std::cout << std::endl;
            std::cout << "Error: Passwords do not match." << std::endl;
            return;
        }
        
        if (userManager.resetPassword(user.getUsername(), newPassword))
        {
            std::cout << std::endl;
            std::cout << "Password reset successful!" << std::endl;
            std::cout << "You can now login with your new password." << std::endl;
        }
        else
        {
            std::cout << std::endl;
            std::cout << "Error: Password reset failed." << std::endl;
        }
    }
    else
    {
        std::cout << std::endl;
        std::cout << "Identity verification failed." << std::endl;
        std::cout << "The email and name combination was not found." << std::endl;
    }
}
