#include "Transaction.h"
#include "CSVReader.h"
#include <sstream>
#include <iomanip>

Transaction::Transaction()
: transactionId(0),
  username(""),
  timestamp(""),
  type(TransactionType::DEPOSIT),
  currency(""),
  amount(0.0),
  balanceAfter(0.0),
  description("")
{
}

Transaction::Transaction(int _transactionId,
                         std::string _username,
                         std::string _timestamp,
                         TransactionType _type,
                         std::string _currency,
                         double _amount,
                         double _balanceAfter,
                         std::string _description)
: transactionId(_transactionId),
  username(_username),
  timestamp(_timestamp),
  type(_type),
  currency(_currency),
  amount(_amount),
  balanceAfter(_balanceAfter),
  description(_description)
{
}

int Transaction::getTransactionId() const
{
    return transactionId;
}

std::string Transaction::getUsername() const
{
    return username;
}

std::string Transaction::getTimestamp() const
{
    return timestamp;
}

TransactionType Transaction::getType() const
{
    return type;
}

std::string Transaction::getCurrency() const
{
    return currency;
}

double Transaction::getAmount() const
{
    return amount;
}

double Transaction::getBalanceAfter() const
{
    return balanceAfter;
}

std::string Transaction::getDescription() const
{
    return description;
}

std::string Transaction::typeToString(TransactionType type)
{
    switch (type)
    {
        case TransactionType::DEPOSIT:
            return "DEPOSIT";
        case TransactionType::WITHDRAW:
            return "WITHDRAW";
        case TransactionType::TRADE_BUY:
            return "TRADE_BUY";
        case TransactionType::TRADE_SELL:
            return "TRADE_SELL";
        default:
            return "UNKNOWN";
    }
}

TransactionType Transaction::stringToType(std::string str)
{
    if (str == "DEPOSIT")
        return TransactionType::DEPOSIT;
    else if (str == "WITHDRAW")
        return TransactionType::WITHDRAW;
    else if (str == "TRADE_BUY")
        return TransactionType::TRADE_BUY;
    else if (str == "TRADE_SELL")
        return TransactionType::TRADE_SELL;
    else
        return TransactionType::DEPOSIT; // default
}

std::string Transaction::toCSVLine() const
{
    std::ostringstream oss;
    oss << transactionId << ","
        << username << ","
        << timestamp << ","
        << typeToString(type) << ","
        << currency << ","
        << std::fixed << std::setprecision(4) << amount << ","
        << std::fixed << std::setprecision(4) << balanceAfter << ","
        << description;
    return oss.str();
}

Transaction Transaction::fromCSVLine(const std::string& line)
{
    std::vector<std::string> tokens = CSVReader::tokenise(line, ',');
    
    if (tokens.size() < 8)
    {
        // Return dummy transaction if invalid format
        return Transaction(0, "", "", TransactionType::DEPOSIT, "", 0.0, 0.0, "");
    }
    
    int id = std::stoi(tokens[0]);
    std::string username = tokens[1];
    std::string timestamp = tokens[2];
    TransactionType type = stringToType(tokens[3]);
    std::string currency = tokens[4];
    double amount = std::stod(tokens[5]);
    double balance = std::stod(tokens[6]);
    std::string description = tokens[7];
    
    return Transaction(id, username, timestamp, type, currency, amount, balance, description);
}

std::string Transaction::toString() const
{
    std::ostringstream oss;
    oss << std::left << std::setw(6) << transactionId
        << std::setw(21) << timestamp
        << std::setw(14) << typeToString(type)
        << std::setw(10) << currency
        << std::right << std::setw(12) << std::fixed << std::setprecision(4) << amount
        << std::setw(12) << std::fixed << std::setprecision(4) << balanceAfter
        << "  " << description;
    return oss.str();
}
