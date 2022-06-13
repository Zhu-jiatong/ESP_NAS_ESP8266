#if !defined(DISPLAY_h)
#define DISPLAY_h

#include "myCfg.h"
#include "webCfg.h"
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display(128, 64, &Wire, -1);

void initOLED()
{
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false);
    display.setTextColor(SSD1306_WHITE);
    display.clearDisplay();
}

void homeRefresh()
{
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.printf("AP_PSK: %s\n", WiFi.softAPPSK());
    yield();
    display.printf("AP_Client: %i\n", WiFi.softAPgetStationNum());
    display.printf("SD: %d Size: %s\n", hasSD, SDsize);
    yield();
    display.printf("STA_IP: %s\n", WiFi.localIP().toString().c_str());
    display.printf("Upload: %s\n", (isUpload ? "Uploading..." : "Idle"));
    display.printf("To Sleep: %i sec", milToS(leftTime));
    display.display();
}

void printd(const char *text)
{
    display.println(text);
    display.display();
}

#endif // DISPLAY_h
