#include "TransactionLogger.h"
#include "CSVReader.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

TransactionLogger::TransactionLogger(std::string _csvFilename)
: csvFilename(_csvFilename), nextTransactionId(1)
{
    loadTransactions();
}

void TransactionLogger::logTransaction(std::string username,
                                       TransactionType type,
                                       std::string currency,
                                       double amount,
                                       double balanceAfter,
                                       std::string description)
{
    std::string timestamp = getCurrentTimestamp();
    
    Transaction transaction(nextTransactionId, username, timestamp, type,
                          currency, amount, balanceAfter, description);
    
    saveTransaction(transaction);
    
    nextTransactionId++;
}

std::vector<Transaction> TransactionLogger::getAllTransactions(std::string username)
{
    std::vector<Transaction> userTransactions;
    
    std::ifstream file(csvFilename);
    if (!file.is_open())
    {
        return userTransactions; // Empty vector
    }
    
    std::string line;
    // Skip header
    std::getline(file, line);
    
    while (std::getline(file, line))
    {
        if (line.empty())
            continue;
            
        try {
            Transaction transaction = Transaction::fromCSVLine(line);
            if (transaction.getUsername() == username)
            {
                userTransactions.push_back(transaction);
            }
        }
        catch (const std::exception& e) {
            // Skip invalid lines
        }
    }
    
    file.close();
    return userTransactions;
}

std::vector<Transaction> TransactionLogger::getRecentTransactions(std::string username, int count)
{
    std::vector<Transaction> allTransactions = getAllTransactions(username);
    
    // Sort by transaction ID (descending) to get most recent first
    std::sort(allTransactions.begin(), allTransactions.end(),
              [](const Transaction& a, const Transaction& b) {
                  return a.getTransactionId() > b.getTransactionId();
              });
    
    // Return only the requested count
    if (allTransactions.size() > static_cast<size_t>(count))
    {
        allTransactions.resize(count);
    }
    
    return allTransactions;
}

std::vector<Transaction> TransactionLogger::getTransactionsByProduct(std::string username, 
                                                                     std::string product)
{
    std::vector<Transaction> allTransactions = getAllTransactions(username);
    std::vector<Transaction> filtered;
    
    for (const Transaction& trans : allTransactions)
    {
        // Check if currency matches or if description contains the product
        if (trans.getCurrency() == product || 
            trans.getDescription().find(product) != std::string::npos)
        {
            filtered.push_back(trans);
        }
    }
    
    return filtered;
}

void TransactionLogger::loadTransactions()
{
    std::ifstream file(csvFilename);
    if (!file.is_open())
    {
        std::cout << "TransactionLogger: No existing transaction file. Will create new one." << std::endl;
        nextTransactionId = 1;
        return;
    }
    
    std::string line;
    int maxId = 0;
    
    // Skip header
    std::getline(file, line);
    
    // Find the highest transaction ID
    while (std::getline(file, line))
    {
        if (line.empty())
            continue;
            
        try {
            Transaction transaction = Transaction::fromCSVLine(line);
            if (transaction.getTransactionId() > maxId)
            {
                maxId = transaction.getTransactionId();
            }
        }
        catch (const std::exception& e) {
            // Skip invalid lines
        }
    }
    
    file.close();
    
    // Set next ID to be one more than the maximum
    nextTransactionId = maxId + 1;
}

void TransactionLogger::saveTransaction(const Transaction& transaction)
{
    // Check if file exists
    std::ifstream checkFile(csvFilename);
    bool fileExists = checkFile.good();
    checkFile.close();
    
    // Open for appending
    std::ofstream file(csvFilename, std::ios::app);
    if (!file.is_open())
    {
        std::cout << "TransactionLogger: Error - Could not open file for writing: " 
                  << csvFilename << std::endl;
        return;
    }
    
    // Write header if file is new
    if (!fileExists)
    {
        file << "transactionId,username,timestamp,type,currency,amount,balance,description" << std::endl;
    }
    
    // Append transaction
    file << transaction.toCSVLine() << std::endl;
    
    file.close();
}

std::string TransactionLogger::getCurrentTimestamp()
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&now_time), "%Y-%m-%d %H:%M:%S");
    return oss.str();
}
