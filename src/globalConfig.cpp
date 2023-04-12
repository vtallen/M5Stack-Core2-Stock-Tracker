#include <Arduino.h>
#include <ArduinoJson.h>
#include <SD.h>

#include <vector>

#include "helpers/Stock.h"

namespace Config {
String g_WifiSsid{};
String g_WifiPassword{};
String g_AlphaVantageKey{};
int g_timeBetweenStockSwitches{};
std::vector<Stock> g_stocks{};

void loadConfig() {
  File file = SD.open("/config.json");

  if (!file) {
    Serial.print("Config::load_config() - Error opening file: /config.json");
    return;
  }

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, file);

  file.close();

  if (error) {
    Serial.print(F("Config::load_config() - Error parsing JSON: "));
    Serial.println(error.c_str());
    return;
  }

  const char *wifi_ssid = doc["WIFI_SSID"];
  const char *wifi_password = doc["WIFI_PASSWORD"];
  const char *alpha_vantage_key = doc["ALPHA_VANTAGE_KEY"];
  const int seconds_between_stock_switches =
      doc["SECONDS_BETWEEN_STOCK_SWITCHES"];

  // Ensures a value was extracted, if it was, then set the member variables
  if (wifi_ssid == nullptr) {
    Serial.println(
        "Config::load_config() - WIFI_SSID not present in config.json");
  } else {
    g_WifiSsid = String(wifi_ssid);
  }

  if (wifi_password == nullptr) {
    Serial.println(
        "Config::load_config() - WIFI_PASSWORD not present in config.json");
  } else {
    g_WifiPassword = String(wifi_password);
  }

  if (alpha_vantage_key == nullptr) {
    Serial.println(
        "Config::load_config() - ALPHA_VANTAGE_KEY not present in config.json");
  } else {
    g_AlphaVantageKey = String(alpha_vantage_key);
  }

  if (seconds_between_stock_switches == 0) {
    Serial.println("Config::load_config() - SECONDS_BETWEEN_STOCK_SWITCHES not "
                   "present in config.json");
    g_timeBetweenStockSwitches = 10000;
  } else {
    // Convert seconds to milliseconds
    g_timeBetweenStockSwitches = seconds_between_stock_switches * 1000;
  }
}

/*
This file contains all of the stocks to be displayed on the screen.

Example JSON document:
[
{
  "ticker": "AAPL",
  "company_name": "Apple",
  "price": 132.03,
  "change_day": -0.81,
  "volume": 88651257
},
{
  "ticker": "GOOGL",
  "company_name":"Google",
  "price": 2112.42,
  "change_day": 14.78,
  "volume": 1162407
}
]
*/
void loadStocksFile() {
  if (!(SD.begin())) {
    Serial.println("Config::load_stocks_file() - Unable to load SD card, card "
                   "failed or not present.");
    return;
  }

  File dataFile = SD.open("/stocks.json", FILE_READ);
  if (!dataFile) {
    Serial.println("Config::load_stocks_file() - Error opening stocks.json");
    return;
  }

  StaticJsonDocument<1024> doc;

  DeserializationError error = deserializeJson(doc, dataFile);
  if (error) {
    Serial.println("Config::load_stocks_file() - Failed to parse JSON!");
    dataFile.close();
    return;
  }

  JsonArray stocks = doc.as<JsonArray>();

  for (JsonObject stock : stocks) {
    g_stocks.push_back(Stock{String(static_cast<const char *>(stock["ticker"])),
                             static_cast<double>(stock["price"])});
  }

  dataFile.close();

  SD.end();
}

void writeStocksFile() {
  SD.begin(4);
  const size_t capacity{JSON_OBJECT_SIZE(g_stocks.size()) *
                        JSON_OBJECT_SIZE(sizeof(Stock))};
  DynamicJsonDocument doc(capacity);

  JsonArray stockArray{doc.to<JsonArray>()};

  for (auto &stock : g_stocks) {
    JsonObject stockObj{stockArray.createNestedObject()};

    stockObj["ticker"] = stock.getTicker();
    stockObj["price"] = stock.getPrice();
  }

  File file = SD.open("/stocks.json", FILE_WRITE);

  if (file) {
    serializeJson(doc, file);
    file.close();
  } else {
    Serial.println("Failed to open file for writing");
  }
}
} // namespace Config