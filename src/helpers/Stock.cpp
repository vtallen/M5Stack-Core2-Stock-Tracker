#include <cassert>

#include "helpers/Stock.h"

bool Stock::operator==(const Stock stock) { return stock.m_ticker == m_ticker; }
bool Stock::operator==(const Stock *stock) {
  return m_ticker == stock->m_ticker;
}

String const &Stock::getCompanyName() { return m_company_name; }

String Stock::getTicker() { return m_ticker; };

double const &Stock::getPrice() { return m_current_price; }

double Stock::getChangeDaily() {
  // Gets the second to last index value from the weekly price sereis which
  // corasponds to the close price of the previous market day
  unsigned int yesterdayCloseIndex{m_weekly_price_series.size() - 2};

  // Checks that there is enough data in the weekly price series to get
  // yesterdays price. If there is not we just return 0
  if (m_weekly_price_series.size() >= 2)
    return m_current_price - m_weekly_price_series[yesterdayCloseIndex];
  else
    return 0.0;
}
std::vector<double> const &Stock::getWeeklyPriceSeries() {
  return m_weekly_price_series;
}

void Stock::setPrice(double price) { m_current_price = price; }
void Stock::setWeeklyPriceSeries(std::vector<double> data) {
  m_weekly_price_series = data;
}