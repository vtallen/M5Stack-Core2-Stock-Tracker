/*

#include <Arduino.h>
#include <M5Core2.h>
#include <lvgl.h>

#include <helpers/Stock.h>

#include <screens/mainScreen.h>

namespace MainScreen {
//Config *config{nullptr};

String ticker{"AAPL"};
String company_name{};
double change{};
double price{198.99};

std::vector<Stock> *stocks {new std::vector<Stock>};

lv_obj_t *ticker_label{nullptr};
lv_obj_t *price_label{nullptr};

lv_chart_series_t *chart_data;

void draw_ticker() {
  if (ticker_label != nullptr) {
    lv_obj_del(ticker_label);
  }

  // Create the label for the stock's ticker
  ticker_label = lv_label_create(lv_scr_act());
  lv_obj_set_style_text_font(ticker_label, &lv_font_montserrat_48,
                             LV_PART_MAIN);
  lv_label_set_text(ticker_label, ticker.c_str());
  lv_obj_align(ticker_label, LV_ALIGN_TOP_LEFT, 10, 5);
}

void draw_price() {
  if (price_label != nullptr) {
    lv_obj_del(price_label);
  }

  // Create the label for the stock's price
  price_label = lv_label_create(lv_scr_act());
  lv_obj_set_style_text_font(price_label, &lv_font_montserrat_48, LV_PART_MAIN);
  lv_label_set_text(price_label, String(price).c_str());
  lv_obj_align(price_label, LV_ALIGN_TOP_LEFT, 10, 55);
}

void update();

void init() {
  // config = config_obj;
  stocks->push_back(Stock{"GGGG", "TEST", 420.69});
  stocks->push_back(Stock{"FFFF", "TEST", 69.69});
  stocks->push_back(Stock{"FFFF", "TEST", 69.69});
  stocks->push_back(Stock{"FFFF", "TEST", 69.69});
  update();

  // Create the chart, this will be moved to a seperate function once
  // the logic for getting the data is implemented
  lv_obj_t *chart = lv_chart_create(lv_scr_act());
  lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
  lv_chart_set_point_count(chart, 7);
  lv_obj_set_size(chart, 300, 120);
  lv_obj_align(chart, LV_ALIGN_BOTTOM_MID, 0, -10);

  chart_data = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED),
                                   LV_CHART_AXIS_PRIMARY_Y);

  lv_chart_set_next_value(chart, chart_data, 10);
  lv_chart_set_next_value(chart, chart_data, 50);
  lv_chart_set_next_value(chart, chart_data, 85);
  lv_chart_set_next_value(chart, chart_data, 30);
  lv_chart_set_next_value(chart, chart_data, 5);
  lv_chart_set_next_value(chart, chart_data, 90);
  lv_chart_set_next_value(chart, chart_data, 75);
  lv_chart_set_next_value(chart, chart_data, 50);
}

void update() {
  // assert(config && "MainScreen::update() - Config object was nullptr. Call "
  //                  "MainScreen::init() and pass in the program's config");

  // static std::vector<Stock> stocks{config->get_stocks()};
  static unsigned long last_update_time{0};
  static std::size_t num_stocks{stocks->size()};
  //Serial.println("NUM STOCKS " + num_stocks);
  static int current_stock{0};

  // TODO add a way to get time between switches to a conf file
  unsigned long current_time{millis()};
  if (((current_time - last_update_time) >= 5000) || (last_update_time == 0)) {
    Serial.println("UPDATE");
    ticker = stocks->at(current_stock).get_ticker();
    company_name = stocks->at(current_stock).get_company_name();
    price = stocks->at(current_stock).get_price();
    Serial.println(ticker);
    Serial.println(company_name);
    Serial.println(price);



    last_update_time = millis();

    if ((current_stock + 1) < num_stocks) {
      ++current_stock;
    } else {
      current_stock = 0;
    }

    draw_ticker();
    draw_price();
  }
}

// Updates the chart
void set_chart_data(double data[]) {
  // Set the num of points
  // Set chart color based on if the stock is up or down
  // Loop over data[] and set each value in the chart
}

void exit() {
  lv_obj_del(ticker_label);
  ticker_label = nullptr;
}
} // namespace MainScreen

*/

#include <Arduino.h>
#include <M5Core2.h>
#include <lvgl.h>

#include "globalConfig.h"

#include "screens/mainScreen.h"

namespace MainScreen {
String ticker{"TEST"};
String companyName{};
String price{};

lv_obj_t *ticker_label;
lv_obj_t *price_label;

void init() {
  ticker_label = lv_label_create(lv_scr_act());
  lv_obj_set_style_text_font(ticker_label, &lv_font_montserrat_48,
                             LV_PART_MAIN);
  lv_obj_align(ticker_label, LV_ALIGN_TOP_LEFT, 10, 5);

  price_label = lv_label_create(lv_scr_act());
  lv_obj_set_style_text_font(price_label, &lv_font_montserrat_48, LV_PART_MAIN);
  lv_obj_align(price_label, LV_ALIGN_TOP_LEFT, 10, 55);
}

void update() {
  static unsigned long lastUpdateTime{0};
  static int currentStockIndex{0};

  if (((millis() - lastUpdateTime) > Config::g_timeBetweenStockSwitches) || lastUpdateTime == 0) {
    unsigned int numStocks{Config::g_stocks.size()};

    ticker = Config::g_stocks.at(currentStockIndex).getTicker();
    lv_label_set_text(ticker_label, ticker.c_str());

    price = Config::g_stocks.at(currentStockIndex).getPrice();
    lv_label_set_text(price_label, String(price).c_str());

    if ((currentStockIndex + 1) < numStocks) {
      ++currentStockIndex;
    } else {
      currentStockIndex = 0;
    }

    lastUpdateTime = millis();
  }
}

} // namespace MainScreen