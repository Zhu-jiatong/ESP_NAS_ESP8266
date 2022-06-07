#include "libs/myCfg.h"
#include "libs/webCfg.h"

void setup(void)
{
    Serial.begin(115600);

    WiFi.mode(WIFI_AP);
    WiFi.softAP(apssid);

    dnsServer.setTTL(300);
    dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
    dnsServer.start(DNS_PORT, "www.pillbox.com", WiFi.softAPIP());

    server.onNotFound([](AsyncWebServerRequest *request)
                      { getPage(request); });
    //server.onFileUpload(handleUpload);
    server.on("/listfiles", listFiles);
    server.begin();

    SD.begin(D8);
}

void loop(void)
{
    dnsServer.processNextRequest();
}