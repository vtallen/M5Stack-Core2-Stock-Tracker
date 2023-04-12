#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>

namespace StockAPI {

String makeRequest(String url) {
  HTTPClient http;
  http.begin(url);

  int httpCode = http.GET();
  String payload{"-1"};
  if (httpCode > 0) {
    payload = http.getString();
  }

  http.end();
  return payload;
}

double getMarketPrice(const String &ticker) {
  String url = "https://query1.finance.yahoo.com/v8/finance/chart/" + ticker +
               "?interval=1d";

  String payload{makeRequest(url)};
  if (payload == "-1")
    return -1.0;

  DynamicJsonDocument doc(1024);
  deserializeJson(doc, payload);

  return doc["chart"]["result"][0]["meta"]["regularMarketPrice"];
}


std::vector<double> getPriceSeriesWeek(const String &ticker) {

  String url = "https://query1.finance.yahoo.com/v8/finance/chart/" + ticker +
               "?interval=1d&range=1wk";
  String payload{makeRequest(url)};

  DynamicJsonDocument doc(2048);
  deserializeJson(doc, payload);

  std::vector<double> priceSeries;

  JsonArray quote_opens =
      doc["chart"]["result"][0]["indicators"]["quote"][0]["close"];
  for (auto value : quote_opens) {
    priceSeries.push_back(value.as<double>());
    // Serial.println(value.as<double>());
  }

  return priceSeries;
}



} // namespace StockAPI