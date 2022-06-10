#if !defined(SLEEPTIMER)
#define SLEEPTIMER

#include <Arduino.h>
#include "myCfg.h"

unsigned long prevTime{};

void updateTimer()
{
    unsigned long nowTime(millis());
    if (!WiFi.softAPgetStationNum() && !WiFi.isConnected())
    {
        auto passTime = nowTime - prevTime;
        if (passTime >= wakeTime)
        {
            prevTime = nowTime;
            display.clearDisplay();
            display.setTextSize(2);
            display.setCursor(0, 0);
            display.print("\n Sleeping\n ...");
            display.display();
            ESP.deepSleep(0);
        }
        leftTime = wakeTime - passTime;
    }
    else
        prevTime = nowTime;
}

#endif // SLEEPTIMER
