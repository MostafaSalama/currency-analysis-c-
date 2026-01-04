#pragma once

#include "User.h"
#include "UserManager.h"
#include <string>

class AuthenticationSystem
{
public:
    /** Constructor */
    AuthenticationSystem();

    /**
     * Main authentication flow
     * Returns authenticated user or empty User if user exits
     */
    User authenticate();

private:
    UserManager userManager;

    /** Print authentication menu */
    void printAuthMenu();

    /** Get user's menu choice */
    int getAuthOption();

    /** Handle login flow - returns authenticated user or empty User */
    User handleLogin();

    /** Handle registration flow - returns authenticated user or empty User */
    User handleRegister();

    /** Handle password reset flow */
    void handlePasswordReset();
};
