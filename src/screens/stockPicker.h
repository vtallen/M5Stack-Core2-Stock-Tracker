#ifndef STOCK_PICKER_H
#define STOCK_PICKER_H
#include <Arduino.h>
namespace StockPicker {
void init();
void exit();
String *get_ticker();
} // namespace StockPicker
#endif