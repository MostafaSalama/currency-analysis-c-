#include "Candlestick.h"
#include <sstream>
#include <iomanip>

Candlestick::Candlestick(std::string _timeframe,
                         double _open,
                         double _high,
                         double _low,
                         double _close,
                         int _volume)
: timeframe(_timeframe),
  open(_open),
  high(_high),
  low(_low),
  close(_close),
  volume(_volume)
{
}

std::string Candlestick::getTimeframe() const
{
    return timeframe;
}

double Candlestick::getOpen() const
{
    return open;
}

double Candlestick::getHigh() const
{
    return high;
}

double Candlestick::getLow() const
{
    return low;
}

double Candlestick::getClose() const
{
    return close;
}

int Candlestick::getVolume() const
{
    return volume;
}

std::string Candlestick::toString() const
{
    std::ostringstream oss;
    oss << std::left << std::setw(20) << timeframe
        << std::right << std::setw(10) << std::fixed << std::setprecision(2) << open
        << std::setw(10) << std::fixed << std::setprecision(2) << high
        << std::setw(10) << std::fixed << std::setprecision(2) << low
        << std::setw(10) << std::fixed << std::setprecision(2) << close
        << std::setw(10) << volume;
    return oss.str();
}

bool Candlestick::compareByTimeframe(const Candlestick& c1, const Candlestick& c2)
{
    return c1.timeframe < c2.timeframe;
}
