#ifndef GLOBALCONFIG_H
#define GLOBALCONFIG_H
#include <vector>

#include "helpers/Stock.h"

namespace Config {
extern String g_WifiSsid;
extern String g_WifiPassword;
extern int g_timeBetweenStockSwitches;
extern std::vector<Stock> g_stocks;

void loadConfig();
void loadStocksFile();
void writeStocksFile();

} // namespace Config
#endif