#pragma once

#include <string>
#include <map>
#include "TransactionLogger.h"

struct UserStatistics
{
    int totalAsks;
    int totalBids;
    std::map<std::string, int> asksByProduct;
    std::map<std::string, int> bidsByProduct;
    double totalMoneySpent;
    int totalDeposits;
    int totalWithdrawals;
    double totalDepositAmount;
    double totalWithdrawalAmount;
};

class StatisticsCalculator
{
public:
    /** Constructor */
    StatisticsCalculator(TransactionLogger& logger);

    /**
     * Calculate statistics for all user activity
     * @param username User's username
     * @return UserStatistics structure with all stats
     */
    UserStatistics calculateStatistics(std::string username);

    /**
     * Calculate statistics for a specific product
     * @param username User's username
     * @param product Product to filter by
     * @return UserStatistics structure with filtered stats
     */
    UserStatistics calculateStatisticsForProduct(std::string username, 
                                                  std::string product);

    /**
     * Display statistics in formatted output
     * @param stats UserStatistics structure to display
     * @param username Username for display
     */
    void displayStatistics(const UserStatistics& stats, std::string username);

    /**
     * Display product-specific statistics
     * @param stats UserStatistics structure to display
     * @param username Username for display
     * @param product Product name for display
     */
    void displayProductStatistics(const UserStatistics& stats, 
                                   std::string username,
                                   std::string product);

private:
    TransactionLogger& transactionLogger;
};
