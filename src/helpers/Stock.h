#ifndef STOCK_H
#define STOCK_H
#include <Arduino.h>
#include <string>
#include <vector>

// struct StockDataPoint {
//   String date;
//   double price;
// };

class Stock {
private:
  String m_company_name{};
  String m_ticker{};
  double m_current_price{};
  std::vector<double> m_weekly_price_series{};

public:
  Stock(String ticker) : m_ticker{ticker} {}

  // Stock(String ticker, String company_name)
  //     : m_ticker{ticker}, m_company_name{company_name} {}

  Stock() : m_ticker{}, m_company_name{}, m_current_price{} {}

  Stock(const String &ticker, double current_price)
      : m_ticker{ticker}, m_current_price{current_price} {}

  bool operator==(const Stock stock);
  bool operator==(const Stock *stock);

  String const &getCompanyName();
  String getTicker();
  double const &getPrice();
  double getChangeDaily();
  std::vector<double> const &getWeeklyPriceSeries();

  void setPrice(double price);
  void setWeeklyPriceSeries(std::vector<double> data);

  ~Stock() {}
};
#endif