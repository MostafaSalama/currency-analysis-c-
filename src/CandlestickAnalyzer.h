#pragma once

#include <vector>
#include <string>
#include "Candlestick.h"
#include "OrderBookEntry.h"

class CandlestickAnalyzer
{
public:
    /** Time interval options for candlestick aggregation */
    enum TimeInterval { SECOND, MINUTE, ALL };

    /** 
     * Generate candlestick data from order book entries
     * @param orders Vector of all orders from the order book
     * @param product The product to filter by (e.g., "ETH/USDT")
     * @param type The order type to filter by (ask or bid)
     * @param interval The time interval for aggregation
     * @return Vector of Candlestick objects representing OHLC data
     */
    static std::vector<Candlestick> generateCandlesticks(
        const std::vector<OrderBookEntry>& orders,
        std::string product,
        OrderBookType type,
        TimeInterval interval
    );

    /**
     * Convert TimeInterval enum to human-readable string
     */
    static std::string intervalToString(TimeInterval interval);

private:
    /**
     * Extract the timeframe key from a timestamp based on the interval
     * @param timestamp Full timestamp (e.g., "2020/03/17 17:01:24.884492")
     * @param interval The time interval for grouping
     * @return Timeframe key (e.g., "2020-03-17 17:01:00" for MINUTE)
     */
    static std::string getTimeframeKey(std::string timestamp, TimeInterval interval);
};
