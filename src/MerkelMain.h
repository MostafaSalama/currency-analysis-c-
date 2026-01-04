#pragma once

#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"
#include "User.h"
#include "WalletManager.h"
#include "TransactionLogger.h"
#include "StatisticsCalculator.h"


class MerkelMain
{
    public:
        MerkelMain(User user);
        /** Call this to start the sim */
        void init();
    private: 
        void printMenu();
        void printHelp();
        void printMarketStats();
        void enterAsk();
        void enterBid();
        void printWallet();
        void gotoNextTimeframe();
        void viewCandlestickAnalysis();
        void manageWallet();
        void depositFunds();
        void withdrawFunds();
        void viewTransactions();
        void viewStatistics();
        int getUserOption();
        void processUserOption(int userOption);

        std::string currentTime;
        User currentUser;
        WalletManager walletManager;
        TransactionLogger transactionLogger;

        // OrderBook orderBook{"20200317.csv"};
	OrderBook orderBook{"20200601.csv"};
        Wallet wallet;

};
