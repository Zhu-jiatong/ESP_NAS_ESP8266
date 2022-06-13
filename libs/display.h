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
    display.printf("AP_SSID: %s\n", WiFi.softAPSSID());
    display.printf("AP_PSK: %s\n", WiFi.softAPPSK().c_str());
    yield();
    display.printf("Client: %i\n", WiFi.softAPgetStationNum());
    display.printf("SD: %d Size: %s\n", hasSD, SDsize);
    display.printf("WiFi: %s\n", (WiFi.getMode() == WIFI_AP ? "WIFI_AP" : WiFi.getMode() == WIFI_AP_STA ? "WIFI_AP_STA"
                                                                      : WiFi.getMode() == WIFI_STA      ? "WIFI_STA"
                                                                                                        : "WIFI_OFF"));
    yield();
    display.printf("STA_IP: %s\n", WiFi.localIP().toString().c_str());
    display.printf("Upload: %s\n", (stage == ING ? "Uploading..." : "Idle :)"));
    display.printf("To Sleep: %i sec", milToS(leftTime));
    display.display();
}

void printd(const char *text)
{
    display.println(text);
    display.display();
}

#endif // DISPLAY_h
