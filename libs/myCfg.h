#if !defined(MYCFG_h)
#define MYCFG_h

#include <DNSServer.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>
#include <SD.h>
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display(128, 64, &Wire, -1);

#define sToMillis(sec) (sec * 1000)
#define milToS(mil) (mil / 1000)

enum uploadStages
{
    ING,
    IDLE
} stage;

bool hasSD{false};
String SDsize{};
unsigned long wakeTime{sToMillis(90)}, leftTime{};

String humanReadableSize(const uint64_t bytes)
{
    if (bytes < 1024)
        return String(bytes) + " B";
    else if (bytes < (1024 * 1024))
        return String(bytes / 1024.0) + " KB";
    else if (bytes < (1024 * 1024 * 1024))
        return String(bytes / 1024.0 / 1024.0) + " MB";
    else
        return String(bytes / 1024.0 / 1024.0 / 1024.0) + " GB";
}

void espSleep()
{
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.print("\n Sleeping\n ...");
    display.display();
    ESP.deepSleep(0);
}

#endif // MYCFG_h
