#include <DFRobot_ID809_I2C.h>

DFRobot_ID809_I2C fingerprint;
#include "libs/webCfg.h"
#include "libs/display.h"

IRAM_ATTR void loadSD()
{
    if (hasSD && stage == IDLE)
    {
        SD.end();
        hasSD = false;
        SDsize = "";
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
    display.println("Initialise Biometric");
    display.display();
    while (!fingerprint.isConnected())
    {
        passMillis = millis() - beginMillis;
        if (passMillis > wakeTime)
        {
            display.println("Biometric failed.");
            display.display();
            delay(3000);
            espSleep();
        }
    }
    fingerprint.ctrlLED(fingerprint.eBreathing, fingerprint.eLEDBlue, 0);
    display.println("Biometric on.");
    display.println("Press finger...");
    display.display();
    if (fingerprint.collectionFingerprint(10))
    {
        display.println("Capture failed.");
        display.display();
        delay(3000);
        fingerprint.ctrlLED(fingerprint.eFadeOut, fingerprint.eLEDBlue, 0);
        espSleep();
    }
    fingerprint.ctrlLED(fingerprint.eFastBlink, fingerprint.eLEDYellow, 3);
    display.println("Fingerprint captured");
    display.display();
    if (!fingerprint.search())
    {
        fingerprint.ctrlLED(fingerprint.eKeepsOn, fingerprint.eLEDRed, 0);
        display.println("Matching failed");
        display.display();
        delay(3000);
        fingerprint.ctrlLED(fingerprint.eFadeOut, fingerprint.eLEDRed, 0);
        espSleep();
    }
    fingerprint.ctrlLED(fingerprint.eKeepsOn, fingerprint.eLEDGreen, 0);
    display.println("Matching success!");
    display.display();
    delay(1000);
    fingerprint.ctrlLED(fingerprint.eFadeOut, fingerprint.eLEDGreen, 0);
}
void setup()
{
    initOLED();
    bioAuth();
    stage = IDLE;
    loadSD();
    attachInterrupt(digitalPinToInterrupt(3), loadSD, FALLING);

    WiFi.mode(WIFI_AP_STA);
    String appsk;
    for (short i = 0; i < 8; ++i)
        appsk += char(random(32, 126));

    WiFi.softAP(apssid, appsk);

    APdnsServer.setTTL(300);
    APdnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
    APdnsServer.start(DNS_PORT, dnsDom, WiFi.softAPIP());

    server.on("/listfiles", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/plain", listFiles()); });
    server.onFileUpload(handleUpload);
    server.on("/file", HTTP_GET, handleFile);
    server.on("/postSTA", HTTP_GET, handleSTA);
    server.onNotFound(handleNotFound);
    server.begin();
}

void loop()
{
    printStr();
    updateTimer();
    APdnsServer.processNextRequest();
}