#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>

namespace StockAPI {
void testParse() {
  const char *json =
      "{\"chart\":{\"result\":[{\"meta\":{\"currency\":\"USD\",\"symbol\":"
      "\"AAPL\",\"exchangeName\":\"NMS\",\"instrumentType\":\"EQUITY\","
      "\"firstTradeDate\":345479400,\"regularMarketTime\":1680636785,"
      "\"gmtoffset\":-14400,\"timezone\":\"EDT\",\"exchangeTimezoneName\":"
      "\"America/"
      "New_York\",\"regularMarketPrice\":165.55,\"chartPreviousClose\":166.17,"
      "\"priceHint\":2,\"currentTradingPeriod\":{\"pre\":{\"timezone\":\"EDT\","
      "\"start\":1680595200,\"end\":1680615000,\"gmtoffset\":-14400},"
      "\"regular\":{\"timezone\":\"EDT\",\"start\":1680615000,\"end\":"
      "1680638400,\"gmtoffset\":-14400},\"post\":{\"timezone\":\"EDT\","
      "\"start\":1680638400,\"end\":1680652800,\"gmtoffset\":-14400}},"
      "\"dataGranularity\":\"1d\",\"range\":\"1d\",\"validRanges\":[\"1d\","
      "\"5d\",\"1mo\",\"3mo\",\"6mo\",\"1y\",\"2y\",\"5y\",\"10y\",\"ytd\","
      "\"max\"]},\"timestamp\":[1680636785],\"indicators\":{\"quote\":[{"
      "\"open\":[166.59500122070312],\"volume\":[32858959],\"high\":[166."
      "8300018310547],\"low\":[165.3300018310547],\"close\":[165.5500030517578]"
      "}],\"adjclose\":[{\"adjclose\":[165.5500030517578]}]}}],\"error\":null}"
      "}";

  DynamicJsonDocument doc(2048);
  DeserializationError error = deserializeJson(doc, json);

  if (error) {
    Serial.print("JSON parsing failed: ");
    Serial.println(error.c_str());
    return;
  }

  JsonObject chart = doc["chart"];
  JsonArray result = chart["result"];
  JsonObject meta = result[0]["meta"];
  double regularMarketPrice = meta["regularMarketPrice"];
  double price = doc["chart"]["result"][0]["meta"]["regularMarketPrice"];

  Serial.print("Regular Market Price: ");
  // Serial.println(regularMarketPrice);
  Serial.println(price);
}

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

double getMarketPrice(String ticker) {
  String url = "https://query1.finance.yahoo.com/v8/finance/chart/" + ticker +
               "?interval=1d";
  // HTTPClient http;
  // http.begin(url);

  // int httpCode = http.GET();
  // double price{-1.0};

  // if (httpCode > 0) {
  //   String payload = http.getString();
  //   DynamicJsonDocument doc(1024);
  //   deserializeJson(doc, payload);

  //   price = doc["chart"]["result"][0]["meta"]["regularMarketPrice"];
  // }

  String payload{makeRequest(url)};
  if (payload == "-1")
    return -1.0;

  DynamicJsonDocument doc(1024);
  deserializeJson(doc, payload);

  return doc["chart"]["result"][0]["meta"]["regularMarketPrice"];
}
} // namespace StockAPI