#include <lvgl.h>
#include <vector>

#include "globalConfig.h"

namespace StockRemover {

bool isInited{false};

struct StockLineItem {
  lv_obj_t *stockLabel{};
  lv_obj_t *deleteButton{};
  lv_obj_t *deleteButtonLabel{};
  String stockTicker{};
  Stock *stock{};
};

std::vector<StockLineItem *> stockLineItems{};

lv_obj_t *backScreen;
lv_obj_t *stockRemoverScreen;

lv_obj_t *cancelBtn;
lv_obj_t *cancelBtnLabel;

void drawLineItems();

void exit() {
  lv_scr_load(backScreen);
  lv_obj_del(stockRemoverScreen);
  stockLineItems.clear();
  isInited = false;
}

// Remember to deallocate the contents of stockLineItems
void deleteBtnCb(lv_event_t *e) {
  // Makes sure the user cannot delete the last stock loaded.
  if (Config::g_stocks.size() == 1) {
    return;
  }

  StockLineItem *currentLineItem{static_cast<StockLineItem *>(e->user_data)};

  auto stockIt{std::find(Config::g_stocks.begin(), Config::g_stocks.end(),
                         currentLineItem->stock)};
  Config::g_stocks.erase(stockIt);

  for (Stock stock : Config::g_stocks) {
    Serial.println();
    Serial.println(stock.getTicker());
  }

  for (StockLineItem *item : stockLineItems) {
    lv_obj_del(item->stockLabel);
    lv_obj_del(
        item->deleteButton); // Also deletes the label because it is a child
  }
  lv_obj_del(cancelBtn);

  stockLineItems.clear();

  drawLineItems();
}

void cancelButtonCb(lv_event_t *e) { exit(); }

void drawLineItems() {
  int yPosAccumulator{0}; // The Y value where new items should be drawn at
  for (int i{0}; i < Config::g_stocks.size(); ++i) {
    yPosAccumulator = (i * 75) - 75;
    StockLineItem *tempLineItem{new StockLineItem};
    tempLineItem->stock = &Config::g_stocks[i];

    tempLineItem->stockLabel = lv_label_create(lv_scr_act());
    lv_label_set_text(tempLineItem->stockLabel,
                      Config::g_stocks[i].getTicker().c_str());
    lv_obj_set_style_text_font(tempLineItem->stockLabel, &lv_font_montserrat_48,
                               LV_PART_MAIN);
    lv_obj_align(tempLineItem->stockLabel, LV_ALIGN_RIGHT_MID, -10,
                 yPosAccumulator);

    tempLineItem->deleteButton = lv_btn_create(lv_scr_act());
    lv_obj_set_size(tempLineItem->deleteButton, 50, 50);

    tempLineItem->deleteButtonLabel =
        lv_label_create(tempLineItem->deleteButton);
    lv_label_set_text(tempLineItem->deleteButtonLabel, "X");
    lv_obj_align(tempLineItem->deleteButtonLabel, LV_ALIGN_CENTER, 0, 0);

    lv_obj_align(tempLineItem->deleteButton, LV_ALIGN_LEFT_MID, 10,
                 yPosAccumulator);

    stockLineItems.push_back(tempLineItem);

    lv_obj_add_event_cb(tempLineItem->deleteButton, deleteBtnCb,
                        LV_EVENT_CLICKED, tempLineItem);
  }
  yPosAccumulator += 75;

  cancelBtn = lv_btn_create(stockRemoverScreen);
  cancelBtnLabel = lv_label_create(cancelBtn);
  lv_obj_add_event_cb(cancelBtn, cancelButtonCb, LV_EVENT_CLICKED, nullptr);

  lv_label_set_text(cancelBtnLabel, "Cancel");
  lv_obj_align(cancelBtnLabel, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_size(cancelBtn, LV_HOR_RES - 10, 50);
  lv_obj_align(cancelBtn, LV_ALIGN_CENTER, 0, yPosAccumulator);
}

void init() {
  if (isInited) {
    return;
  }

  isInited = true;
  Serial.println("HERE");
  backScreen = lv_scr_act();
  stockRemoverScreen = lv_obj_create(nullptr);

  lv_scr_load(stockRemoverScreen);

  drawLineItems();
}
} // namespace StockRemover