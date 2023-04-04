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
  // std::vector<StockDataPoint> m_historical_data{};

public:
  Stock(String ticker) : m_ticker{ticker} {}
  Stock(String ticker, String company_name)
      : m_ticker{ticker}, m_company_name{company_name} {}
  Stock() : m_ticker{}, m_company_name{}, m_current_price{} {};

  Stock(String ticker, String company_name, double current_price)
      : m_ticker{ticker}, m_company_name{company_name}, m_current_price{
                                                            current_price} {}

  String const &getCompanyName();
  String const &getTicker();
  double const &getPrice();
  // const std::vector<StockDataPoint> &get_historicl_data();

  ~Stock() {}
};
#endif