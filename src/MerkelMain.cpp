#include "MerkelMain.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include "CandlestickAnalyzer.h"

MerkelMain::MerkelMain(User user)
: currentUser(user)
{

}

void MerkelMain::init()
{
    int input;
    currentTime = orderBook.getEarliestTime();

    // Try to load existing wallet, or initialize with defaults
    if (!walletManager.loadWallet(currentUser.getUsername(), wallet))
    {
        // New wallet - initialize with default balance
        wallet.insertCurrency("BTC", 10);
        wallet.insertCurrency("USDT", 10000);
        wallet.insertCurrency("ETH", 100);
        
        // Log initial balances
        transactionLogger.logTransaction(currentUser.getUsername(), 
                                         TransactionType::DEPOSIT, "BTC", 10.0, 10.0, 
                                         "Initial balance");
        transactionLogger.logTransaction(currentUser.getUsername(), 
                                         TransactionType::DEPOSIT, "USDT", 10000.0, 10000.0, 
                                         "Initial balance");
        transactionLogger.logTransaction(currentUser.getUsername(), 
                                         TransactionType::DEPOSIT, "ETH", 100.0, 100.0, 
                                         "Initial balance");
        
        // Save initial wallet
        walletManager.saveWallet(currentUser.getUsername(), wallet);
    }

    // Welcome message
    std::cout << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Welcome to MerkelRex, " << currentUser.getFullName() << "!" << std::endl;
    std::cout << "Username: " << currentUser.getUsername() << std::endl;
    std::cout << "========================================" << std::endl;

    while(true)
    {
        printMenu();
        input = getUserOption();
        processUserOption(input);
    }
}


void MerkelMain::printMenu()
{
    // 1 print help
    std::cout << "1: Print help " << std::endl;
    // 2 print exchange stats
    std::cout << "2: Print exchange stats" << std::endl;
    // 3 make an offer
    std::cout << "3: Make an offer " << std::endl;
    // 4 make a bid 
    std::cout << "4: Make a bid " << std::endl;
    // 5 wallet management
    std::cout << "5: Wallet Management " << std::endl;
    // 6 continue   
    std::cout << "6: Continue " << std::endl;
    // 7 view candlestick analysis
    std::cout << "7: View Candlestick Analysis " << std::endl;

    std::cout << "============== " << std::endl;

    std::cout << "Current time is: " << currentTime << std::endl;
}

void MerkelMain::printHelp()
{
    std::cout << "Help - your aim is to make money. Analyse the market and make bids and offers. " << std::endl;
}

void MerkelMain::printMarketStats()
{
    for (std::string const& p : orderBook.getKnownProducts())
    {
        std::cout << "Product: " << p << std::endl;
        std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask, 
                                                                p, currentTime);
        std::cout << "Asks seen: " << entries.size() << std::endl;
        std::cout << "Max ask: " << OrderBook::getHighPrice(entries) << std::endl;
        std::cout << "Min ask: " << OrderBook::getLowPrice(entries) << std::endl;



    }
    // std::cout << "OrderBook contains :  " << orders.size() << " entries" << std::endl;
    // unsigned int bids = 0;
    // unsigned int asks = 0;
    // for (OrderBookEntry& e : orders)
    // {
    //     if (e.orderType == OrderBookType::ask)
    //     {
    //         asks ++;
    //     }
    //     if (e.orderType == OrderBookType::bid)
    //     {
    //         bids ++;
    //     }  
    // }    
    // std::cout << "OrderBook asks:  " << asks << " bids:" << bids << std::endl;

}

void MerkelMain::enterAsk()
{
    std::cout << "Make an ask - enter the amount: product,price, amount, eg  ETH/BTC,200,0.5" << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "MerkelMain::enterAsk Bad input! " << input << std::endl;
    }
    else {
        try {
            OrderBookEntry obe = CSVReader::stringsToOBE(
                tokens[1],
                tokens[2], 
                currentTime, 
                tokens[0], 
                OrderBookType::ask 
            );
            obe.username = currentUser.getUsername();
            if (wallet.canFulfillOrder(obe))
            {
                std::cout << "Wallet looks good. " << std::endl;
                orderBook.insertOrder(obe);
            }
            else {
                std::cout << "Wallet has insufficient funds . " << std::endl;
            }
        }catch (const std::exception& e)
        {
            std::cout << " MerkelMain::enterAsk Bad input " << std::endl;
        }   
    }
}

void MerkelMain::enterBid()
{
    std::cout << "Make an bid - enter the amount: product,price, amount, eg  ETH/BTC,200,0.5" << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "MerkelMain::enterBid Bad input! " << input << std::endl;
    }
    else {
        try {
            OrderBookEntry obe = CSVReader::stringsToOBE(
                tokens[1],
                tokens[2], 
                currentTime, 
                tokens[0], 
                OrderBookType::bid 
            );
            obe.username = currentUser.getUsername();

            if (wallet.canFulfillOrder(obe))
            {
                std::cout << "Wallet looks good. " << std::endl;
                orderBook.insertOrder(obe);
            }
            else {
                std::cout << "Wallet has insufficient funds . " << std::endl;
            }
        }catch (const std::exception& e)
        {
            std::cout << " MerkelMain::enterBid Bad input " << std::endl;
        }   
    }
}

void MerkelMain::printWallet()
{
    std::cout << wallet.toString() << std::endl;
}
        
void MerkelMain::gotoNextTimeframe()
{
    std::cout << "Going to next time frame. " << std::endl;
    for (std::string p : orderBook.getKnownProducts())
    {
        std::cout << "matching " << p << std::endl;
        std::vector<OrderBookEntry> sales =  orderBook.matchAsksToBids(p, currentTime);
        std::cout << "Sales: " << sales.size() << std::endl;
        for (OrderBookEntry& sale : sales)
        {
            std::cout << "Sale price: " << sale.price << " amount " << sale.amount << std::endl; 
            if (sale.username == currentUser.getUsername())
            {
                // update the wallet
                wallet.processSale(sale);
                
                // Log the trade transaction
                std::vector<std::string> currs = CSVReader::tokenise(sale.product, '/');
                std::string currency = currs[0];
                
                // Get updated balance for this currency
                std::map<std::string, double> currencies = wallet.getAllCurrencies();
                double balance = 0.0;
                if (currencies.find(currency) != currencies.end())
                {
                    balance = currencies[currency];
                }
                
                if (sale.orderType == OrderBookType::asksale)
                {
                    // User sold currency (ask filled)
                    transactionLogger.logTransaction(
                        currentUser.getUsername(),
                        TransactionType::TRADE_SELL,
                        currency,
                        sale.amount,
                        balance,
                        "Sold " + sale.product + " at " + std::to_string(sale.price)
                    );
                }
                else if (sale.orderType == OrderBookType::bidsale)
                {
                    // User bought currency (bid filled)
                    transactionLogger.logTransaction(
                        currentUser.getUsername(),
                        TransactionType::TRADE_BUY,
                        currency,
                        sale.amount,
                        balance,
                        "Bought " + sale.product + " at " + std::to_string(sale.price)
                    );
                }
                
                // Save updated wallet
                walletManager.saveWallet(currentUser.getUsername(), wallet);
            }
        }
        
    }

    currentTime = orderBook.getNextTime(currentTime);
}

void MerkelMain::viewCandlestickAnalysis()
{
    std::cout << "=== Candlestick Analysis ===" << std::endl;
    
    // Step 1: Get product from user
    std::cout << "Enter product (e.g., ETH/USDT, ETH/BTC, DOGE/BTC): ";
    std::string product;
    std::getline(std::cin, product);
    
    // Validate product exists
    std::vector<std::string> knownProducts = orderBook.getKnownProducts();
    bool productFound = false;
    for (const std::string& p : knownProducts)
    {
        if (p == product)
        {
            productFound = true;
            break;
        }
    }
    
    if (!productFound)
    {
        std::cout << "Error: Product '" << product << "' not found in the order book." << std::endl;
        std::cout << "Available products: ";
        for (size_t i = 0; i < knownProducts.size(); ++i)
        {
            std::cout << knownProducts[i];
            if (i < knownProducts.size() - 1)
                std::cout << ", ";
        }
        std::cout << std::endl;
        return;
    }
    
    // Step 2: Get time interval from user
    std::cout << "Select time interval:" << std::endl;
    std::cout << "  1: Per Second" << std::endl;
    std::cout << "  2: Per Minute" << std::endl;
    std::cout << "  3: All Time" << std::endl;
    std::cout << "Enter choice (1-3): ";
    std::string intervalInput;
    std::getline(std::cin, intervalInput);
    
    CandlestickAnalyzer::TimeInterval interval;
    int intervalChoice = 0;
    try {
        intervalChoice = std::stoi(intervalInput);
    } catch(const std::exception& e) {
        std::cout << "Invalid interval choice." << std::endl;
        return;
    }
    
    switch (intervalChoice)
    {
        case 1:
            interval = CandlestickAnalyzer::SECOND;
            break;
        case 2:
            interval = CandlestickAnalyzer::MINUTE;
            break;
        case 3:
            interval = CandlestickAnalyzer::ALL;
            break;
        default:
            std::cout << "Invalid interval choice. Please select 1, 2, or 3." << std::endl;
            return;
    }
    
    // Step 3: Get all orders from the order book
    std::vector<OrderBookEntry> allOrders = orderBook.getOrders(OrderBookType::ask, product, currentTime);
    // For analysis, we need all orders, not just current time
    // Let's get the orders differently - we need access to all orders
    // Since OrderBook doesn't expose all orders, we'll need to work with what we have
    
    // Generate candlesticks for ASKS
    std::cout << "\n=== Candlestick Analysis: " << product << " (ASKS) ===" << std::endl;
    std::cout << "Interval: " << CandlestickAnalyzer::intervalToString(interval) << std::endl;
    std::cout << std::endl;
    
    // We need to collect all orders for this product across all timestamps
    // Let's modify approach - collect all unique timestamps first
    std::vector<std::string> timestamps;
    std::string ts = orderBook.getEarliestTime();
    timestamps.push_back(ts);
    
    // Collect all timestamps
    for (int i = 0; i < 1000; ++i) // arbitrary limit to prevent infinite loop
    {
        std::string nextTs = orderBook.getNextTime(ts);
        if (nextTs == timestamps[0]) // wrapped around
            break;
        timestamps.push_back(nextTs);
        ts = nextTs;
    }
    
    // Collect all orders for this product
    std::vector<OrderBookEntry> allAsks;
    std::vector<OrderBookEntry> allBids;
    
    for (const std::string& timestamp : timestamps)
    {
        std::vector<OrderBookEntry> asks = orderBook.getOrders(OrderBookType::ask, product, timestamp);
        std::vector<OrderBookEntry> bids = orderBook.getOrders(OrderBookType::bid, product, timestamp);
        allAsks.insert(allAsks.end(), asks.begin(), asks.end());
        allBids.insert(allBids.end(), bids.begin(), bids.end());
    }
    
    // Generate candlesticks for ASKS
    std::vector<Candlestick> askCandlesticks = CandlestickAnalyzer::generateCandlesticks(
        allAsks, product, OrderBookType::ask, interval);
    
    if (askCandlesticks.empty())
    {
        std::cout << "No ask data available for " << product << std::endl;
    }
    else
    {
        // Print header
        std::cout << std::left << std::setw(20) << "Timeframe"
                  << std::right << std::setw(10) << "Open"
                  << std::setw(10) << "High"
                  << std::setw(10) << "Low"
                  << std::setw(10) << "Close"
                  << std::setw(10) << "Volume" << std::endl;
        std::cout << std::string(70, '-') << std::endl;
        
        // Print each candlestick
        for (const Candlestick& candle : askCandlesticks)
        {
            std::cout << candle.toString() << std::endl;
        }
    }
    
    // Generate candlesticks for BIDS
    std::cout << "\n=== Candlestick Analysis: " << product << " (BIDS) ===" << std::endl;
    std::cout << "Interval: " << CandlestickAnalyzer::intervalToString(interval) << std::endl;
    std::cout << std::endl;
    
    std::vector<Candlestick> bidCandlesticks = CandlestickAnalyzer::generateCandlesticks(
        allBids, product, OrderBookType::bid, interval);
    
    if (bidCandlesticks.empty())
    {
        std::cout << "No bid data available for " << product << std::endl;
    }
    else
    {
        // Print header
        std::cout << std::left << std::setw(20) << "Timeframe"
                  << std::right << std::setw(10) << "Open"
                  << std::setw(10) << "High"
                  << std::setw(10) << "Low"
                  << std::setw(10) << "Close"
                  << std::setw(10) << "Volume" << std::endl;
        std::cout << std::string(70, '-') << std::endl;
        
        // Print each candlestick
        for (const Candlestick& candle : bidCandlesticks)
        {
            std::cout << candle.toString() << std::endl;
        }
    }
    
    std::cout << std::endl;
}
 
int MerkelMain::getUserOption()
{
    int userOption = 0;
    std::string line;
    std::cout << "Type in 1-7" << std::endl;
    std::getline(std::cin, line);
    try{
        userOption = std::stoi(line);
    }catch(const std::exception& e)
    {
        // 
    }
    std::cout << "You chose: " << userOption << std::endl;
    return userOption;
}

void MerkelMain::processUserOption(int userOption)
{
    if (userOption == 0) // bad input
    {
        std::cout << "Invalid choice. Choose 1-7" << std::endl;
    }
    if (userOption == 1) 
    {
        printHelp();
    }
    if (userOption == 2) 
    {
        printMarketStats();
    }
    if (userOption == 3) 
    {
        enterAsk();
    }
    if (userOption == 4) 
    {
        enterBid();
    }
    if (userOption == 5) 
    {
        manageWallet();
    }
    if (userOption == 6) 
    {
        gotoNextTimeframe();
    }
    if (userOption == 7)
    {
        viewCandlestickAnalysis();
    }
}

void MerkelMain::manageWallet()
{
    while (true)
    {
        std::cout << std::endl;
        std::cout << "=== Wallet Management ===" << std::endl;
        std::cout << "1: View Balance" << std::endl;
        std::cout << "2: Deposit Funds" << std::endl;
        std::cout << "3: Withdraw Funds" << std::endl;
        std::cout << "4: View Recent Transactions" << std::endl;
        std::cout << "5: View Statistics" << std::endl;
        std::cout << "6: Back to Main Menu" << std::endl;
        std::cout << "=========================" << std::endl;
        
        int option = 0;
        std::string line;
        std::cout << "Enter your choice (1-6): ";
        std::getline(std::cin, line);
        
        try {
            option = std::stoi(line);
        } catch (const std::exception& e) {
            option = 0;
        }
        
        std::cout << "You chose: " << option << std::endl;
        
        if (option == 1)
        {
            printWallet();
        }
        else if (option == 2)
        {
            depositFunds();
        }
        else if (option == 3)
        {
            withdrawFunds();
        }
        else if (option == 4)
        {
            viewTransactions();
        }
        else if (option == 5)
        {
            viewStatistics();
        }
        else if (option == 6)
        {
            break; // Return to main menu
        }
        else
        {
            std::cout << "Invalid choice. Please select 1-6." << std::endl;
        }
    }
}

void MerkelMain::depositFunds()
{
    std::cout << std::endl;
    std::cout << "=== Deposit Funds ===" << std::endl;
    
    std::string currency;
    std::cout << "Enter currency (e.g., BTC, USDT, ETH): ";
    std::getline(std::cin, currency);
    
    std::string amountStr;
    std::cout << "Enter amount to deposit: ";
    std::getline(std::cin, amountStr);
    
    try {
        double amount = std::stod(amountStr);
        
        if (amount <= 0)
        {
            std::cout << "Error: Amount must be positive." << std::endl;
            return;
        }
        
        // Add to wallet
        wallet.insertCurrency(currency, amount);
        
        // Get new balance
        std::map<std::string, double> currencies = wallet.getAllCurrencies();
        double newBalance = currencies[currency];
        
        // Log transaction
        transactionLogger.logTransaction(currentUser.getUsername(),
                                         TransactionType::DEPOSIT,
                                         currency,
                                         amount,
                                         newBalance,
                                         "Manual deposit");
        
        // Save wallet
        walletManager.saveWallet(currentUser.getUsername(), wallet);
        
        std::cout << std::endl;
        std::cout << "Deposit successful!" << std::endl;
        std::cout << "New " << currency << " balance: " << newBalance << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Error: Invalid amount." << std::endl;
    }
}

void MerkelMain::withdrawFunds()
{
    std::cout << std::endl;
    std::cout << "=== Withdraw Funds ===" << std::endl;
    
    std::string currency;
    std::cout << "Enter currency (e.g., BTC, USDT, ETH): ";
    std::getline(std::cin, currency);
    
    std::string amountStr;
    std::cout << "Enter amount to withdraw: ";
    std::getline(std::cin, amountStr);
    
    try {
        double amount = std::stod(amountStr);
        
        if (amount <= 0)
        {
            std::cout << "Error: Amount must be positive." << std::endl;
            return;
        }
        
        // Check if sufficient balance
        if (!wallet.containsCurrency(currency, amount))
        {
            std::cout << "Error: Insufficient balance." << std::endl;
            return;
        }
        
        // Remove from wallet
        if (wallet.removeCurrency(currency, amount))
        {
            // Get new balance
            std::map<std::string, double> currencies = wallet.getAllCurrencies();
            double newBalance = 0.0;
            if (currencies.find(currency) != currencies.end())
            {
                newBalance = currencies[currency];
            }
            
            // Log transaction
            transactionLogger.logTransaction(currentUser.getUsername(),
                                             TransactionType::WITHDRAW,
                                             currency,
                                             amount,
                                             newBalance,
                                             "Manual withdrawal");
            
            // Save wallet
            walletManager.saveWallet(currentUser.getUsername(), wallet);
            
            std::cout << std::endl;
            std::cout << "Withdrawal successful!" << std::endl;
            std::cout << "New " << currency << " balance: " << newBalance << std::endl;
        }
        else
        {
            std::cout << "Error: Withdrawal failed." << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cout << "Error: Invalid amount." << std::endl;
    }
}

void MerkelMain::viewTransactions()
{
    std::cout << std::endl;
    std::cout << "=== Transaction History ===" << std::endl;
    std::cout << "1: View Recent Transactions (last 5)" << std::endl;
    std::cout << "2: View All Transactions" << std::endl;
    std::cout << "3: View by Currency" << std::endl;
    std::cout << "============================" << std::endl;
    
    int option = 0;
    std::string line;
    std::cout << "Enter your choice (1-3): ";
    std::getline(std::cin, line);
    
    try {
        option = std::stoi(line);
    } catch (const std::exception& e) {
        option = 0;
    }
    
    std::vector<Transaction> transactions;
    
    if (option == 1)
    {
        transactions = transactionLogger.getRecentTransactions(currentUser.getUsername(), 5);
        std::cout << std::endl;
        std::cout << "=== Recent Transactions (Last 5) ===" << std::endl;
    }
    else if (option == 2)
    {
        transactions = transactionLogger.getAllTransactions(currentUser.getUsername());
        std::cout << std::endl;
        std::cout << "=== All Transactions ===" << std::endl;
    }
    else if (option == 3)
    {
        std::string currency;
        std::cout << "Enter currency or product: ";
        std::getline(std::cin, currency);
        
        transactions = transactionLogger.getTransactionsByProduct(currentUser.getUsername(), currency);
        std::cout << std::endl;
        std::cout << "=== Transactions for " << currency << " ===" << std::endl;
    }
    else
    {
        std::cout << "Invalid choice." << std::endl;
        return;
    }
    
    if (transactions.empty())
    {
        std::cout << "No transactions found." << std::endl;
        return;
    }
    
    std::cout << "User: " << currentUser.getFullName() << " (" << currentUser.getUsername() << ")" << std::endl;
    std::cout << std::endl;
    std::cout << std::left << std::setw(6) << "ID"
              << std::setw(21) << "Timestamp"
              << std::setw(14) << "Type"
              << std::setw(10) << "Currency"
              << std::right << std::setw(12) << "Amount"
              << std::setw(12) << "Balance"
              << "  Description" << std::endl;
    std::cout << std::string(100, '-') << std::endl;
    
    for (const Transaction& trans : transactions)
    {
        std::cout << trans.toString() << std::endl;
    }
    std::cout << std::endl;
}

void MerkelMain::viewStatistics()
{
    std::cout << std::endl;
    std::cout << "=== Statistics ===" << std::endl;
    std::cout << "1: View All Statistics" << std::endl;
    std::cout << "2: View Statistics by Product" << std::endl;
    std::cout << "==================" << std::endl;
    
    int option = 0;
    std::string line;
    std::cout << "Enter your choice (1-2): ";
    std::getline(std::cin, line);
    
    try {
        option = std::stoi(line);
    } catch (const std::exception& e) {
        option = 0;
    }
    
    StatisticsCalculator statsCalc(transactionLogger);
    
    if (option == 1)
    {
        UserStatistics stats = statsCalc.calculateStatistics(currentUser.getUsername());
        statsCalc.displayStatistics(stats, currentUser.getFullName() + " (" + currentUser.getUsername() + ")");
        
        // Also show current wallet
        std::cout << "Current Wallet Balance:" << std::endl;
        std::cout << wallet.toString() << std::endl;
    }
    else if (option == 2)
    {
        std::string product;
        std::cout << "Enter product (e.g., ETH/USDT): ";
        std::getline(std::cin, product);
        
        UserStatistics stats = statsCalc.calculateStatisticsForProduct(currentUser.getUsername(), product);
        statsCalc.displayProductStatistics(stats, currentUser.getFullName() + " (" + currentUser.getUsername() + ")", product);
    }
    else
    {
        std::cout << "Invalid choice." << std::endl;
    }
}
