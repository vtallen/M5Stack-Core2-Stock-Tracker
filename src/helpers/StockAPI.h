#ifndef STOCKAPI_H
#define STOCKAPI_H

#include <Arduino.h>

#include "globalConfig.h"

namespace StockAPI {
void testParse();

double getMarketPrice(String ticker);
void getTimeSeriesWeek();
} // namespace StockAPI

#endif