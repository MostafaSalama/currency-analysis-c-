#pragma once

#include <string>

enum class TransactionType {
    DEPOSIT,
    WITHDRAW,
    TRADE_BUY,
    TRADE_SELL
};

class Transaction
{
public:
    /** Default constructor */
    Transaction();

    /** Constructor to create a transaction */
    Transaction(int transactionId,
                std::string username,
                std::string timestamp,
                TransactionType type,
                std::string currency,
                double amount,
                double balanceAfter,
                std::string description);

    /** Get the transaction ID */
    int getTransactionId() const;

    /** Get the username */
    std::string getUsername() const;

    /** Get the timestamp */
    std::string getTimestamp() const;

    /** Get the transaction type */
    TransactionType getType() const;

    /** Get the currency */
    std::string getCurrency() const;

    /** Get the amount */
    double getAmount() const;

    /** Get the balance after transaction */
    double getBalanceAfter() const;

    /** Get the description */
    std::string getDescription() const;

    /** Convert transaction type to string */
    static std::string typeToString(TransactionType type);

    /** Convert string to transaction type */
    static TransactionType stringToType(std::string str);

    /** Convert transaction to CSV line */
    std::string toCSVLine() const;

    /** Create transaction from CSV line */
    static Transaction fromCSVLine(const std::string& line);

    /** Get formatted string representation */
    std::string toString() const;

private:
    int transactionId;
    std::string username;
    std::string timestamp;
    TransactionType type;
    std::string currency;
    double amount;
    double balanceAfter;
    std::string description;
};
