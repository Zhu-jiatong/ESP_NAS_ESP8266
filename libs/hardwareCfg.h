#if !defined(HARDCFG_h)
#define HARDCFG_h

#include <Arduino.h>
#include "display.h"
#include "myCfg.h"
#include <DFRobot_ID809_I2C.h>
DFRobot_ID809_I2C fingerprint;

void espSleep()
{
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.print("\n Sleeping\n ...");
    display.display();
    ESP.deepSleep(0);
}

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

IRAM_ATTR void loadSD()
{
    if (hasSD && !isUpload)
    {
        SD.end();
        hasSD = false;
        SDsize = "NULL";
    }
    else if (!hasSD)
    {
        hasSD = SD.begin(D8, SPI_FULL_SPEED);
        SDsize = humanReadableSize(SD.size64());
    }
}

void bioAuth()
{
    unsigned long beginMillis{millis()};
    unsigned long passMillis{};
    fingerprint.begin();
    display.clearDisplay();
    printd("Initialise Biometric");
    while (!fingerprint.isConnected())
    {
        passMillis = millis() - beginMillis;
        if (passMillis > wakeTime)
        {
            printd("Biometric failed.");
            delay(1000);
            espSleep();
        }
    }
    fingerprint.ctrlLED(fingerprint.eBreathing, fingerprint.eLEDBlue, 0);
    printd("Press finger...");
    if (fingerprint.collectionFingerprint(10))
    {
        printd("Capture failed.");
        delay(1000);
        fingerprint.ctrlLED(fingerprint.eFadeOut, fingerprint.eLEDBlue, 0);
        espSleep();
    }
    fingerprint.ctrlLED(fingerprint.eFastBlink, fingerprint.eLEDYellow, 3);
    printd("Fingerprint captured");
    if (!fingerprint.search())
    {
        fingerprint.ctrlLED(fingerprint.eKeepsOn, fingerprint.eLEDRed, 0);
        printd("Matching failed");
        delay(1000);
        fingerprint.ctrlLED(fingerprint.eFadeOut, fingerprint.eLEDRed, 0);
        espSleep();
    }
    fingerprint.ctrlLED(fingerprint.eKeepsOn, fingerprint.eLEDGreen, 0);
    printd("Matching success!");
    fingerprint.ctrlLED(fingerprint.eFadeOut, fingerprint.eLEDGreen, 0);
}

#endif // HARDCFG_h
