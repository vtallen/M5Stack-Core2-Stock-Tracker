#include <Arduino.h>
#include <M5Core2.h>
#include <lvgl.h>
#include <vector>

#include "globalConfig.h"

#include "screens/mainScreen.h"
#include "screens/stockPicker.h"

namespace MainScreen {
String ticker{};
String price{};
String dailyChange{};

lv_obj_t *ticker_label;
lv_obj_t *price_label;
lv_obj_t *dailyChangeLabel;

lv_obj_t *price_chart;
lv_chart_series_t *price_chart_series;

// lv_obj_t *add_btn;
// lv_obj_t *add_btn_label;

static void add_btn_event_handler(lv_event_t *e) { StockPicker::init(); }

void drawChart(std::vector<double> chartData, bool isChangePositive) {
  if (chartData.size() > 0) {
    lv_chart_set_point_count(price_chart, chartData.size());

    // Find the min and max value within the chart data so that we can set the
    // scale of the chart
    double min_value = *std::min_element(chartData.begin(), chartData.end());
    double max_value = *std::max_element(chartData.begin(), chartData.end());

    lv_chart_set_range(price_chart, LV_CHART_AXIS_PRIMARY_Y, min_value,
                       max_value);

    // Set the color of the chart based on if the stock has lost or gained value
    // in the past week
    if (isChangePositive) {
      lv_chart_set_series_color(price_chart, price_chart_series,
                                lv_palette_main(LV_PALETTE_GREEN));
    } else {
      lv_chart_set_series_color(price_chart, price_chart_series,
                                lv_palette_main(LV_PALETTE_RED));
    }

    for (int dataPointIndex{}; dataPointIndex < chartData.size();
         ++dataPointIndex) {
      price_chart_series->y_points[dataPointIndex] = chartData[dataPointIndex];
    }
  } else {
    Serial.println("MainScreen::drawChart() - Stock has no historical data");
  }
}

void init() {
  ticker_label = lv_label_create(lv_scr_act());
  lv_obj_set_style_text_font(ticker_label, &lv_font_montserrat_48,
                             LV_PART_MAIN);
  lv_obj_align(ticker_label, LV_ALIGN_TOP_LEFT, 10, 5);

  price_label = lv_label_create(lv_scr_act());
  lv_obj_set_style_text_font(price_label, &lv_font_montserrat_48, LV_PART_MAIN);
  lv_obj_align(price_label, LV_ALIGN_TOP_LEFT, 10, 55);

  dailyChangeLabel = lv_label_create(lv_scr_act());
  lv_obj_set_style_text_font(dailyChangeLabel, &lv_font_montserrat_24, LV_PART_MAIN);
  lv_obj_align(dailyChangeLabel, LV_ALIGN_TOP_RIGHT, -10, 76);

  price_chart = lv_chart_create(lv_scr_act());
  price_chart_series = lv_chart_add_series(
      price_chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
  lv_obj_align(price_chart, LV_ALIGN_BOTTOM_MID, 0, -5);
  lv_obj_set_size(price_chart, LV_HOR_RES - 10, 120);
  lv_chart_set_type(price_chart, LV_CHART_TYPE_LINE);
}

void update() {
  static unsigned long lastUpdateTime{0};
  static int currentStockIndex{0};

  if (((millis() - lastUpdateTime) > Config::g_timeBetweenStockSwitches) ||
      lastUpdateTime == 0) {
    unsigned int numStocks{Config::g_stocks.size()};
    Stock &currentStock {Config::g_stocks.at(currentStockIndex)};
    
    double currentStockDailyChange {currentStock.getChangeDaily()};

    ticker = currentStock.getTicker();
    lv_label_set_text(ticker_label, ticker.c_str());

    price = currentStock.getPrice();
    lv_label_set_text(price_label, String(price).c_str());

    dailyChange = String(currentStockDailyChange);
    lv_label_set_text(dailyChangeLabel, dailyChange.c_str());
    // Determine if the stock is up or down, then set the color of the change label accordingly
    auto dailyChangeColor {(currentStockDailyChange > 0)? lv_palette_main(LV_PALETTE_GREEN) : lv_palette_main(LV_PALETTE_RED)};
    lv_obj_set_style_text_color(dailyChangeLabel, dailyChangeColor, LV_PART_MAIN);

    drawChart(currentStock.getWeeklyPriceSeries(), (currentStockDailyChange > 0));

    if ((currentStockIndex + 1) < numStocks) {
      ++currentStockIndex;
    } else {
      currentStockIndex = 0;
    }

    lastUpdateTime = millis();
  }
}

} // namespace MainScreen