#include "libs/webCfg.h"
#include "libs/display.h"

void setup()
{
    hasSD = SD.begin(D8);
    SDsize = humanReadableSize(SD.size64());
    initOLED();
    stage = IDLE;

    WiFi.mode(WIFI_AP_STA);
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
    APdnsServer.processNextRequest();
}