#pragma once

#include <string>

class Candlestick
{
public:
    /** Constructor to create a Candlestick object */
    Candlestick(std::string _timeframe,
                double _open,
                double _high,
                double _low,
                double _close,
                int _volume);

    /** Get the timeframe for this candlestick */
    std::string getTimeframe() const;

    /** Get the opening price */
    double getOpen() const;

    /** Get the highest price */
    double getHigh() const;

    /** Get the lowest price */
    double getLow() const;

    /** Get the closing price */
    double getClose() const;

    /** Get the volume (number of orders) */
    int getVolume() const;

    /** Return a formatted string representation of the candlestick */
    std::string toString() const;

    /** Static comparison function to sort by timeframe */
    static bool compareByTimeframe(const Candlestick& c1, const Candlestick& c2);

private:
    std::string timeframe;  // e.g., "2020-03-17 17:01:00"
    double open;            // First trading price in the timeframe
    double high;            // Highest price in the timeframe
    double low;             // Lowest price in the timeframe
    double close;           // Last trading price in the timeframe
    int volume;             // Number of orders in the timeframe
};
