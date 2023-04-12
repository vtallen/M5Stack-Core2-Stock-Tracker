#ifndef STOCKAPI_H
#define STOCKAPI_H

#include <Arduino.h>

#include "globalConfig.h"

namespace StockAPI {
double getMarketPrice(const String &ticker);
std::vector<double> getPriceSeriesWeek(const String &ticker);
} // namespace StockAPI

#endif