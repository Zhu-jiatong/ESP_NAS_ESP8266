#if !defined(MYCFG_h)
#define MYCFG_h

#include <DNSServer.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>
#include <SD.h>

enum uploadStages
{
    START,
    ING,
    IDLE
} stage;

#endif // MYCFG_h
