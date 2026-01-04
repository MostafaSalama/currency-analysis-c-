#pragma once

#include <string>
#include <map>
#include "Wallet.h"

class WalletManager
{
public:
    /** Constructor - loads wallets from CSV file */
    WalletManager(std::string csvFilename = "wallets.csv");

    /**
     * Load wallet for specific user
     * @param username User's username
     * @param wallet Reference to wallet object to populate
     * @return true if wallet was loaded, false if new wallet
     */
    bool loadWallet(std::string username, Wallet& wallet);

    /**
     * Save wallet for specific user
     * @param username User's username
     * @param wallet Wallet object to save
     * @return true if save successful
     */
    bool saveWallet(std::string username, const Wallet& wallet);

    /**
     * Check if user has saved wallet
     * @param username User's username
     * @return true if wallet exists
     */
    bool hasWallet(std::string username);

private:
    std::string csvFilename;
    std::map<std::string, std::map<std::string, double>> walletData;

    /** Load all wallets from CSV */
    void loadAllWallets();

    /** Save all wallets to CSV */
    void saveAllWallets();
};
