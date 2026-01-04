#include "StatisticsCalculator.h"
#include <iostream>
#include <iomanip>

StatisticsCalculator::StatisticsCalculator(TransactionLogger& logger)
: transactionLogger(logger)
{
}

UserStatistics StatisticsCalculator::calculateStatistics(std::string username)
{
    UserStatistics stats;
    stats.totalAsks = 0;
    stats.totalBids = 0;
    stats.totalMoneySpent = 0.0;
    stats.totalDeposits = 0;
    stats.totalWithdrawals = 0;
    stats.totalDepositAmount = 0.0;
    stats.totalWithdrawalAmount = 0.0;

    // Get all transactions for user
    std::vector<Transaction> transactions = transactionLogger.getAllTransactions(username);

    for (const Transaction& trans : transactions)
    {
        TransactionType type = trans.getType();
        std::string description = trans.getDescription();

        switch (type)
        {
            case TransactionType::TRADE_SELL:
                stats.totalAsks++;
                // Extract product from description if available
                if (description.find("/") != std::string::npos)
                {
                    size_t pos = description.find(" ");
                    if (pos != std::string::npos)
                    {
                        std::string product = description.substr(pos + 1);
                        stats.asksByProduct[product]++;
                    }
                }
                break;

            case TransactionType::TRADE_BUY:
                stats.totalBids++;
                stats.totalMoneySpent += trans.getAmount() * 1.0; // Simplified
                // Extract product from description if available
                if (description.find("/") != std::string::npos)
                {
                    size_t pos = description.find(" ");
                    if (pos != std::string::npos)
                    {
                        std::string product = description.substr(pos + 1);
                        stats.bidsByProduct[product]++;
                    }
                }
                break;

            case TransactionType::DEPOSIT:
                stats.totalDeposits++;
                stats.totalDepositAmount += trans.getAmount();
                break;

            case TransactionType::WITHDRAW:
                stats.totalWithdrawals++;
                stats.totalWithdrawalAmount += trans.getAmount();
                break;
        }
    }

    return stats;
}

UserStatistics StatisticsCalculator::calculateStatisticsForProduct(std::string username, 
                                                                     std::string product)
{
    UserStatistics stats;
    stats.totalAsks = 0;
    stats.totalBids = 0;
    stats.totalMoneySpent = 0.0;
    stats.totalDeposits = 0;
    stats.totalWithdrawals = 0;
    stats.totalDepositAmount = 0.0;
    stats.totalWithdrawalAmount = 0.0;

    // Get transactions filtered by product
    std::vector<Transaction> transactions = transactionLogger.getTransactionsByProduct(username, product);

    for (const Transaction& trans : transactions)
    {
        TransactionType type = trans.getType();

        if (type == TransactionType::TRADE_SELL)
        {
            stats.totalAsks++;
            stats.asksByProduct[product]++;
        }
        else if (type == TransactionType::TRADE_BUY)
        {
            stats.totalBids++;
            stats.bidsByProduct[product]++;
            stats.totalMoneySpent += trans.getAmount();
        }
    }

    return stats;
}

void StatisticsCalculator::displayStatistics(const UserStatistics& stats, std::string username)
{
    std::cout << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "=== Trading Statistics ===" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "User: " << username << std::endl;
    std::cout << "Period: All Time" << std::endl;
    std::cout << std::endl;

    std::cout << "Trading Activity:" << std::endl;
    std::cout << "  Total Asks:  " << stats.totalAsks << std::endl;
    std::cout << "  Total Bids:  " << stats.totalBids << std::endl;
    std::cout << std::endl;

    if (!stats.asksByProduct.empty() || !stats.bidsByProduct.empty())
    {
        std::cout << "By Product:" << std::endl;
        
        // Collect all unique products
        std::map<std::string, bool> allProducts;
        for (const auto& pair : stats.asksByProduct)
            allProducts[pair.first] = true;
        for (const auto& pair : stats.bidsByProduct)
            allProducts[pair.first] = true;

        for (const auto& productPair : allProducts)
        {
            std::string product = productPair.first;
            int asks = 0;
            int bids = 0;

            auto askIt = stats.asksByProduct.find(product);
            if (askIt != stats.asksByProduct.end())
                asks = askIt->second;

            auto bidIt = stats.bidsByProduct.find(product);
            if (bidIt != stats.bidsByProduct.end())
                bids = bidIt->second;

            std::cout << "  " << product << " - Asks: " << asks 
                      << ", Bids: " << bids << std::endl;
        }
        std::cout << std::endl;
    }

    std::cout << "Financial Summary:" << std::endl;
    std::cout << "  Total Money Spent: " << std::fixed << std::setprecision(2) 
              << stats.totalMoneySpent << std::endl;
    std::cout << "  Total Deposits:  " << stats.totalDeposits 
              << " (" << std::fixed << std::setprecision(2) 
              << stats.totalDepositAmount << ")" << std::endl;
    std::cout << "  Total Withdrawals: " << stats.totalWithdrawals 
              << " (" << std::fixed << std::setprecision(2) 
              << stats.totalWithdrawalAmount << ")" << std::endl;
    std::cout << std::endl;
}

void StatisticsCalculator::displayProductStatistics(const UserStatistics& stats, 
                                                     std::string username,
                                                     std::string product)
{
    std::cout << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "=== Trading Statistics for " << product << " ===" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "User: " << username << std::endl;
    std::cout << std::endl;

    std::cout << "Trading Activity:" << std::endl;
    std::cout << "  Total Asks:  " << stats.totalAsks << std::endl;
    std::cout << "  Total Bids:  " << stats.totalBids << std::endl;
    std::cout << "  Total Money Spent: " << std::fixed << std::setprecision(2) 
              << stats.totalMoneySpent << std::endl;
    std::cout << std::endl;
}
