#include "helpers/Stock.h"

String const &Stock::getCompanyName() { return m_company_name; }
String const &Stock::getTicker() { return m_ticker; };
double const &Stock::getPrice() { return m_current_price; };