#include "CandlestickAnalyzer.h"
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>

std::vector<Candlestick> CandlestickAnalyzer::generateCandlesticks(
    const std::vector<OrderBookEntry>& orders,
    std::string product,
    OrderBookType type,
    TimeInterval interval)
{
    // Map to group orders by timeframe
    // Key: timeframe string, Value: vector of prices for that timeframe
    std::map<std::string, std::vector<double>> timeframeData;
    
    // Filter and group orders by timeframe
    for (const OrderBookEntry& order : orders)
    {
        // Filter by product and type
        if (order.product == product && order.orderType == type)
        {
            std::string timeframeKey = getTimeframeKey(order.timestamp, interval);
            timeframeData[timeframeKey].push_back(order.price);
        }
    }
    
    // Generate candlestick objects from grouped data
    std::vector<Candlestick> candlesticks;
    
    for (const auto& pair : timeframeData)
    {
        std::string timeframe = pair.first;
        const std::vector<double>& prices = pair.second;
        
        if (prices.empty())
            continue;
        
        // Calculate OHLC values
        double open = prices.front();  // First price
        double close = prices.back();  // Last price
        double high = *std::max_element(prices.begin(), prices.end());
        double low = *std::min_element(prices.begin(), prices.end());
        int volume = prices.size();
        
        Candlestick candlestick(timeframe, open, high, low, close, volume);
        candlesticks.push_back(candlestick);
    }
    
    // Sort candlesticks by timeframe
    std::sort(candlesticks.begin(), candlesticks.end(), Candlestick::compareByTimeframe);
    
    return candlesticks;
}

std::string CandlestickAnalyzer::getTimeframeKey(std::string timestamp, TimeInterval interval)
{
    // Timestamp format: "2020/03/17 17:01:24.884492"
    // Need to convert to: "2020-03-17 HH:MM:SS" or "2020-03-17 HH:MM" or "2020-03-17"
    
    if (timestamp.length() < 10)
        return timestamp;
    
    // Replace / with -
    std::string result = timestamp;
    for (size_t i = 0; i < result.length(); ++i)
    {
        if (result[i] == '/')
            result[i] = '-';
    }
    
    switch (interval)
    {
        case SECOND:
            // Return format: "2020-03-17 17:01:24"
            if (result.length() >= 19)
                return result.substr(0, 19);
            break;
            
        case MINUTE:
            // Return format: "2020-03-17 17:01"
            if (result.length() >= 16)
                return result.substr(0, 16);
            break;
            
        case ALL:
            // Return format: "2020-03-17" (just the date)
            if (result.length() >= 10)
                return result.substr(0, 10);
            break;
    }
    
    return result;
}

std::string CandlestickAnalyzer::intervalToString(TimeInterval interval)
{
    switch (interval)
    {
        case SECOND:
            return "Per Second";
        case MINUTE:
            return "Per Minute";
        case ALL:
            return "All Time";
        default:
            return "Unknown";
    }
}
