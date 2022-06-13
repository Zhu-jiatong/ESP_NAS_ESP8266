#include "libs/webCfg.h"
#include "libs/hardwareCfg.h"

void setup()
{
    initOLED();
    bioAuth();
    stage = IDLE;
    loadSD();
    attachInterrupt(digitalPinToInterrupt(3), loadSD, FALLING);

    startNetwork();
    startServer();

    prevTime = millis();
}

void loop()
{
    homeRefresh();
    updateTimer();
    APdnsServer.processNextRequest();
}