#include <MUIU8g2.h>
#include <U8g2lib.h>
#include <U8x8lib.h>

#include "libs/webCfg.h"
FSInfo64 info_64;

short cycle{};

void setup()
{
    SD.begin(D8);

    WiFi.mode(WIFI_AP);
    WiFi.softAP(apssid);

    dnsServer.setTTL(300);
    dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
    dnsServer.start(DNS_PORT, "www.nas.com", WiFi.softAPIP());

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
    dnsServer.processNextRequest();
}