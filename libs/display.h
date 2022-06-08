#if !defined(DISPLAY_h)
#define DISPLAY_h

#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include "myCfg.h"
Adafruit_SSD1306 display(128, 64, &Wire, -1);

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
    display.printf("SSID: %s\n", WiFi.softAPSSID());
    display.printf("PSK: %s\n", WiFi.softAPPSK().c_str());
    display.printf("Domain: %s\n", dnsDom);
    yield();
    display.printf("Client: %i\n", WiFi.softAPgetStationNum());
    display.printf("Upload: %s\n", (stage == START ? "Starting upload..." : stage == ING ? "Uploading..."
                                                                                         : "Idle :)"));
    display.printf("Reboot: %i", needReboot);
    display.display();
}
#endif // DISPLAY_h
