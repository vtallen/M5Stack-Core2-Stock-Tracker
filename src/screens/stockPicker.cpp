#include <M5Core2.h>
#include <lvgl.h>
#include <string>

#include "helpers/Stock.h"
#include "helpers/StockAPI.h"

#include "globalConfig.h"

namespace StockPicker {
// The screen to load when this screen is exited and the currently active screen
lv_obj_t *back_screen;
lv_obj_t *stock_picker_screen;

// The screen that will be loaded upon the exit of this screen
#pragma GCC diagnostic ignored "-Wwrite-strings"
constexpr char *letters{"-\n"
                        "A\n"
                        "B\n"
                        "C\n"
                        "D\n"
                        "E\n"
                        "F\n"
                        "G\n"
                        "H\n"
                        "I\n"
                        "J\n"
                        "K\n"
                        "L\n"
                        "M\n"
                        "N\n"
                        "O\n"
                        "P\n"
                        "Q\n"
                        "R\n"
                        "S\n"
                        "T\n"
                        "U\n"
                        "V\n"
                        "W\n"
                        "X\n"
                        "Y\n"
                        "Z"};

// Container for the roller objects + the rollers
lv_obj_t *roller_parent;
lv_obj_t *roller1;
lv_obj_t *roller2;
lv_obj_t *roller3;
lv_obj_t *roller4;
lv_obj_t *roller5;

std::string *ticker_value{};

bool isInited{false};

void exit() {
  lv_scr_load(back_screen);
  lv_obj_del(stock_picker_screen);
  isInited = false;
  delete ticker_value;
}

static void add_btn_cb(lv_event_t *event) {
  String tickerString{ticker_value->c_str()};
  Config::g_stocks.push_back(
      Stock{tickerString, StockAPI::getMarketPrice(tickerString)});
  exit();
}

static void cancel_btn_cb(lv_event_t *event) { exit(); }

static void roller_cb(lv_event_t *event) {
  lv_obj_t *rollers[] = {roller1, roller2, roller3, roller4, roller5};
  ticker_value->clear();

  for (int i{0}; i < (sizeof(rollers) / sizeof(rollers[0])); ++i) {
    char buf[2];
    lv_roller_get_selected_str(rollers[i], buf, sizeof(buf));
    if (buf[0] != '-') {
      ticker_value->append(buf);
    }
  }
}

void init() {
  // Return if we have already created the screen. This prevents memory leaks
  if (isInited) {
    return;
  }

  isInited = true;
  ticker_value = new std::string;
  back_screen = lv_scr_act();

  stock_picker_screen = lv_obj_create(nullptr);
  roller_parent = lv_obj_create(stock_picker_screen);

  lv_scr_load(stock_picker_screen);

  lv_obj_set_size(stock_picker_screen, 320, 240);
  lv_obj_set_size(roller_parent, 320, 240);

  roller1 = lv_roller_create(roller_parent);
  roller2 = lv_roller_create(roller_parent);
  roller3 = lv_roller_create(roller_parent);
  roller4 = lv_roller_create(roller_parent);
  roller5 = lv_roller_create(roller_parent);

  constexpr int offset_x{23};
  lv_obj_align_to(roller1, roller_parent, LV_ALIGN_CENTER, -100 + offset_x,
                  -20);
  lv_obj_align_to(roller2, roller_parent, LV_ALIGN_CENTER, -50 + offset_x, -20);
  lv_obj_align_to(roller3, roller_parent, LV_ALIGN_CENTER, 0 + offset_x, -20);
  lv_obj_align_to(roller5, roller_parent, LV_ALIGN_CENTER, 100 + offset_x, -20);
  lv_obj_align_to(roller4, roller_parent, LV_ALIGN_CENTER, 50 + offset_x, -20);

  lv_roller_set_options(roller1, letters, LV_ROLLER_MODE_INFINITE);
  lv_roller_set_options(roller2, letters, LV_ROLLER_MODE_INFINITE);
  lv_roller_set_options(roller3, letters, LV_ROLLER_MODE_INFINITE);
  lv_roller_set_options(roller4, letters, LV_ROLLER_MODE_INFINITE);
  lv_roller_set_options(roller5, letters, LV_ROLLER_MODE_INFINITE);

  lv_obj_add_event_cb(roller1, roller_cb, LV_EVENT_VALUE_CHANGED, (int *)1);
  lv_obj_add_event_cb(roller2, roller_cb, LV_EVENT_VALUE_CHANGED, (int *)2);
  lv_obj_add_event_cb(roller3, roller_cb, LV_EVENT_VALUE_CHANGED, (int *)3);
  lv_obj_add_event_cb(roller4, roller_cb, LV_EVENT_VALUE_CHANGED, (int *)4);
  lv_obj_add_event_cb(roller5, roller_cb, LV_EVENT_VALUE_CHANGED, (int *)5);

  // Create the add button
  lv_obj_t *add_btn_label;

  lv_obj_t *add_btn{lv_btn_create(lv_scr_act())};
  lv_obj_set_size(add_btn, 100, 50);
  lv_obj_align(add_btn, LV_ALIGN_BOTTOM_RIGHT, -10, -10);

  add_btn_label = lv_label_create(add_btn);
  lv_label_set_text(add_btn_label, "ADD");
  lv_obj_center(add_btn_label);

  lv_obj_add_event_cb(add_btn, add_btn_cb, LV_EVENT_CLICKED, nullptr);

  // Create the cancel button
  lv_obj_t *cancel_btn_label;

  lv_obj_t *cancel_btn{lv_btn_create(lv_scr_act())};
  lv_obj_set_size(cancel_btn, 100, 50);
  lv_obj_align(cancel_btn, LV_ALIGN_BOTTOM_LEFT, 10, -10);

  cancel_btn_label = lv_label_create(cancel_btn);
  lv_label_set_text(cancel_btn_label, "CANCEL");
  lv_obj_center(cancel_btn_label);

  lv_obj_add_event_cb(cancel_btn, cancel_btn_cb, LV_EVENT_CLICKED, nullptr);
}

std::string *get_ticker() {
  assert(ticker_value && "StockPicker::get_ticker() was called, but the value "
                         "of ticker_value is nullptr");
  return ticker_value;
}
} // namespace StockPicker