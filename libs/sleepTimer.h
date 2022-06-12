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
            espSleep();
        }
        leftTime = wakeTime - passTime;
    }
    else
        prevTime = nowTime;
}

#endif // SLEEPTIMER
