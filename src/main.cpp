// https://query1.finance.yahoo.com/v8/finance/chart/AAPL?interval=1d&range=1wk
#include <Arduino.h>
#include <M5Core2.h>
#include <SPI.h>
#include <WiFi.h>
#include <Wire.h>
#include <lvgl.h>

#include "globalConfig.h"

#include "helpers/StockAPI.h"

#include "screens/mainScreen.h"
#include "screens/stockPicker.h"
#include "screens/stockRemover.h"

#define LV_HOR_RES_MAX 320
#define LV_VER_RES_MAX 240

// init the tft espi
static lv_disp_draw_buf_t draw_buf;
static lv_disp_drv_t disp_drv;   // Descriptor of a display driver
static lv_indev_drv_t indev_drv; // Descriptor of a touch driver

M5Display *tft;

static void ta_event_cb(lv_event_t *e);
static lv_obj_t *kb;

static void ta_event_cb(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *ta = lv_event_get_target(e);
  if (code == LV_EVENT_CLICKED || code == LV_EVENT_FOCUSED) {
    /*Focus on the clicked text area*/
    if (kb != NULL)
      lv_keyboard_set_textarea(kb, ta);
  }

  else if (code == LV_EVENT_READY) {
    LV_LOG_USER("Ready, current text: %s", lv_textarea_get_text(ta));
  }
}

static void btnPowerOff_event(lv_event_t *event) { M5.Axp.PowerOff(); }

void tft_lv_initialization() {
  M5.begin();
  lv_init();

  static lv_color_t buf1[(LV_HOR_RES_MAX * LV_VER_RES_MAX) /
                         10]; // Declare a buffer for 1/10 screen siz
  // static lv_color_t buf2[(LV_HOR_RES_MAX * LV_VER_RES_MAX) / 10];

  // Initialize `disp_buf` display buffer with the buffer(s).
  lv_disp_draw_buf_init(&draw_buf, buf1, nullptr,
                        (LV_HOR_RES_MAX * LV_VER_RES_MAX) / 10);

  tft = &M5.Lcd;
}

// Display flushing
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area,
                   lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft->startWrite();
  tft->setAddrWindow(area->x1, area->y1, w, h);
  tft->pushColors((uint16_t *)&color_p->full, w * h, true);
  tft->endWrite();

  lv_disp_flush_ready(disp);
}

void init_disp_driver() {
  lv_disp_drv_init(&disp_drv); // Basic initialization

  disp_drv.flush_cb = my_disp_flush; // Set your driver function
  disp_drv.draw_buf = &draw_buf;     // Assign the buffer to the display
  disp_drv.hor_res =
      LV_HOR_RES_MAX; // Set the horizontal resolution of the display
  disp_drv.ver_res =
      LV_VER_RES_MAX; // Set the vertical resolution of the display

  lv_disp_drv_register(&disp_drv); // Finally register the driver
  lv_disp_set_bg_color(
      NULL, lv_color_hex3(0x000)); // Set default background color to black
}

void my_touchpad_read(lv_indev_drv_t *drv, lv_indev_data_t *data) {
  TouchPoint_t pos = M5.Touch.getPressPoint();
  bool touched = (pos.x == -1) ? false : true;

  if (!touched) {
    data->state = LV_INDEV_STATE_RELEASED;
  } else {
    data->state = LV_INDEV_STATE_PRESSED;
    data->point.x = pos.x;
    data->point.y = pos.y;
  }
}

void init_touch_driver() {
  lv_disp_drv_register(&disp_drv);

  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_t *my_indev = lv_indev_drv_register(&indev_drv); // register
}

void setup() {
  Serial.begin(115200);

  tft_lv_initialization();
  init_disp_driver();
  init_touch_driver();

  Config::loadConfig();
  Config::loadStocksFile();

  WiFi.begin(Config::g_WifiSsid.c_str(), Config::g_WifiPassword.c_str());
  Serial.println(Config::g_WifiSsid.c_str());
  Serial.println(Config::g_WifiPassword.c_str());

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  MainScreen::init();
}

void loop() {
  M5.update();
  lv_task_handler();

  if (Config::g_stocks.size() > 0) {
    // Bring up the stock picker if the rightmost capacitive button is pressed.
    if (M5.BtnA.wasPressed()) {
      StockRemover::init();
    } else if (M5.BtnB.wasPressed()) {

    } else if (M5.BtnC.wasPressed()) {
      StockPicker::init();
    }

    // StockAPI request scheduling code
    constexpr int APICallsPerMinute{5};
    // It takes 2 API calls to Yahoo finance in order to update the data of one
    // stock. Therefore we can use the below formula to calculate the number of
    // milliseconds there needs to be between updating each stock.
    static const int millisBetweenAPICalls{60000 / (APICallsPerMinute / 2)};

    static unsigned long lastAPICallTime{0};
    static int currentStockIndex{0};

    if ((millis() - lastAPICallTime) > millisBetweenAPICalls) {
      // First check that the stock we are going to update next has not been
      // removed, if it has, start back at the beginning.
      if (!(currentStockIndex > Config::g_stocks.size())) {

        Stock &currentStock{Config::g_stocks.at(currentStockIndex)};
        currentStock.setPrice(
            StockAPI::getMarketPrice(currentStock.getTicker()));
        currentStock.setWeeklyPriceSeries(
            StockAPI::getPriceSeriesWeek(currentStock.getTicker()));

        // Making sure the next stock exists
        if ((currentStockIndex + 1) < Config::g_stocks.size()) {
          ++currentStockIndex;
        } else {
          currentStockIndex = 0;
        }
      } else {
        currentStockIndex = 0;
      }

      lastAPICallTime = millis();
    }

  } else {
    StockPicker::init();
  }
  MainScreen::update();
}