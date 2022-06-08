#include "libs/webCfg.h"
#include "libs/display.h"

void setup()
{
    SD.begin(D8);

    initOLED();
    stage = IDLE;

    WiFi.mode(WIFI_AP);
    WiFi.softAP(apssid);

    dnsServer.setTTL(300);
    dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
    dnsServer.start(DNS_PORT, dnsDom, WiFi.softAPIP());

    /* server.onNotFound([](AsyncWebServerRequest *request)
                      { getPage(request); }); */
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SDFS, "/index.html", "text/html", false, processor); });
    server.serveStatic("/", SDFS, "/");
    server.on("/listfiles", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/plain", listFiles()); });
    server.onFileUpload(handleUpload);
    server.on("/file", HTTP_GET, handleFile);
    server.onNotFound(handleNotFound);
    server.begin();
}

void loop()
{
    printStr();
    dnsServer.processNextRequest();
}