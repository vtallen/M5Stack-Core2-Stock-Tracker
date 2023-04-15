# A Stock Tracking app for the M5Stack Core 2
---
This is a simple application written for the ESP32 micro controller that uses Yahoo Finance to display the price of stocks on a screen.


<img src="images/main_screen.jpg?raw=true" width=50% height=50%>

## Setup and installation
---
1. Clone this repository to your computer
2. Open the folder with Visual Studio Code. The appropriate extensions and libraries should automatically get installed.
3. Copy lv_conf.h from the root folder into .pio/libdeps/lvgl

Now we need to setup the SD card, the WiFi SSID and password as well as the list of stocks get stored here.
1. On the root of your SD card create a file called config.json
3. Paste the following text into the file and replace YOUR_SSID and YOUR_PASSWORD with the ssid and password for your WiFi network. The SECONDS_BETWEEN_STOCK_SWITCHES value can be changed to modify how long each stock is displayed on screen.
```
{
  "WIFI_SSID": "YOUR_SSID",
  "WIFI_PASSWORD": "YOUR_PASSWORD",
  "SECONDS_BETWEEN_STOCK_SWITCHES": 5
}
```

Finally, connect your M5Core2 and and select the upload button in VS Code.

## Usage
---
Upon first boot you will be asked to select a stock ticker. Use the rollers to enter it and click add. It will take a moment for the graphs to be filled in as the project is setup to only make 5 API calls per minute. This helps mitigate your IP address getting rate limited by Yahoo.

To add another stock, click the rightmost button on the M5Core2. To remove stocks, click the leftmost button.
