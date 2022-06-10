#if !defined(DISPLAY_h)
#define DISPLAY_h

#include "myCfg.h"
#include "sleepTimer.h"

void initOLED()
{
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
}

void printStr()
{
    display.clearDisplay();
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
    display.printf("Sleep: %i sec", milToS(leftTime));
    display.display();
}
#endif // DISPLAY_h
