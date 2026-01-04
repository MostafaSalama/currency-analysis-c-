#include "WalletManager.h"
#include "CSVReader.h"
#include <fstream>
#include <iostream>
#include <sstream>

WalletManager::WalletManager(std::string _csvFilename)
: csvFilename(_csvFilename)
{
    loadAllWallets();
}

bool WalletManager::loadWallet(std::string username, Wallet& wallet)
{
    if (walletData.find(username) != walletData.end())
    {
        // User has saved wallet - load it
        wallet.setCurrencies(walletData[username]);
        return true;
    }
    
    // No saved wallet - will use default initialization
    return false;
}

bool WalletManager::saveWallet(std::string username, const Wallet& wallet)
{
    // Update wallet data in memory
    walletData[username] = wallet.getAllCurrencies();
    
    // Save all wallets to CSV
    saveAllWallets();
    
    return true;
}

bool WalletManager::hasWallet(std::string username)
{
    return walletData.find(username) != walletData.end();
}

void WalletManager::loadAllWallets()
{
    walletData.clear();
    
    std::ifstream file(csvFilename);
    if (!file.is_open())
    {
        std::cout << "WalletManager: No existing wallet file found. Will create new one." << std::endl;
        return;
    }
    
    std::string line;
    std::vector<std::string> currencies;
    
    // Read header to get currency names
    if (std::getline(file, line))
    {
        std::vector<std::string> headers = CSVReader::tokenise(line, ',');
        // First column is username, rest are currencies
        for (size_t i = 1; i < headers.size(); ++i)
        {
            currencies.push_back(headers[i]);
        }
    }
    
    // Read wallet data
    while (std::getline(file, line))
    {
        if (line.empty())
            continue;
            
        std::vector<std::string> tokens = CSVReader::tokenise(line, ',');
        
        if (tokens.size() < 1)
            continue;
            
        std::string username = tokens[0];
        std::map<std::string, double> userWallet;
        
        // Parse currency balances
        for (size_t i = 1; i < tokens.size() && i - 1 < currencies.size(); ++i)
        {
            try {
                double balance = std::stod(tokens[i]);
                userWallet[currencies[i - 1]] = balance;
            }
            catch (const std::exception& e) {
                userWallet[currencies[i - 1]] = 0.0;
            }
        }
        
        walletData[username] = userWallet;
    }
    
    file.close();
    std::cout << "WalletManager: Loaded " << walletData.size() << " wallets." << std::endl;
}

void WalletManager::saveAllWallets()
{
    std::ofstream file(csvFilename);
    if (!file.is_open())
    {
        std::cout << "WalletManager: Error - Could not open file for writing: " 
                  << csvFilename << std::endl;
        return;
    }
    
    // Collect all unique currencies across all wallets
    std::map<std::string, bool> allCurrencies;
    for (const auto& userPair : walletData)
    {
        for (const auto& currencyPair : userPair.second)
        {
            allCurrencies[currencyPair.first] = true;
        }
    }
    
    // Write header
    file << "username";
    for (const auto& currencyPair : allCurrencies)
    {
        file << "," << currencyPair.first;
    }
    file << std::endl;
    
    // Write each user's wallet
    for (const auto& userPair : walletData)
    {
        std::string username = userPair.first;
        const std::map<std::string, double>& userWallet = userPair.second;
        
        file << username;
        
        for (const auto& currencyPair : allCurrencies)
        {
            std::string currency = currencyPair.first;
            double balance = 0.0;
            
            // Check if user has this currency
            auto it = userWallet.find(currency);
            if (it != userWallet.end())
            {
                balance = it->second;
            }
            
            file << "," << balance;
        }
        
        file << std::endl;
    }
    
    file.close();
}
