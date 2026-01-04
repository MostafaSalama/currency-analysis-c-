#pragma once

#include <string>
#include <vector>
#include "Transaction.h"

class TransactionLogger
{
public:
    /** Constructor - loads transactions from CSV file */
    TransactionLogger(std::string csvFilename = "transactions.csv");

    /**
     * Log a new transaction
     * @param username User's username
     * @param type Transaction type
     * @param currency Currency involved
     * @param amount Amount of transaction
     * @param balanceAfter Balance after transaction
     * @param description Optional description
     */
    void logTransaction(std::string username,
                       TransactionType type,
                       std::string currency,
                       double amount,
                       double balanceAfter,
                       std::string description = "");

    /**
     * Get all transactions for a user
     * @param username User's username
     * @return Vector of all transactions
     */
    std::vector<Transaction> getAllTransactions(std::string username);

    /**
     * Get recent transactions for a user
     * @param username User's username
     * @param count Number of recent transactions to return
     * @return Vector of recent transactions (newest first)
     */
    std::vector<Transaction> getRecentTransactions(std::string username, int count = 5);

    /**
     * Get transactions by product/currency
     * @param username User's username
     * @param product Product or currency to filter by
     * @return Vector of filtered transactions
     */
    std::vector<Transaction> getTransactionsByProduct(std::string username, 
                                                      std::string product);

private:
    std::string csvFilename;
    int nextTransactionId;

    /** Load existing transactions to determine next ID */
    void loadTransactions();

    /** Save a single transaction (append to file) */
    void saveTransaction(const Transaction& transaction);

    /** Get current timestamp in standard format */
    std::string getCurrentTimestamp();
};
