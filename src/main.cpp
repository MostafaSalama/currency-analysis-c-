#include "Wallet.h"
#include <iostream>
#include "MerkelMain.h"
#include "AuthenticationSystem.h"

int main()
{   
    // Authenticate user first
    AuthenticationSystem auth;
    User currentUser = auth.authenticate();
    
    // Check if user chose to exit
    if (currentUser.getUsername().empty())
    {
        return 0;
    }
    
    // Start main application with authenticated user
    MerkelMain app{currentUser};
    app.init();
    
    return 0;
}
