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

bool hasSD{false};
String SDsize{};

String humanReadableSize(const uint64_t bytes)
{
    if (bytes < 1024)
        return String(bytes) + " B";
    else if (bytes < (1024 * 1024))
        return String(bytes / 1024.0) + " KB";
    else if (bytes < (1024 * 1024 * 1024))
        return String(bytes / 1024.0 / 1024.0) + " MB";
    else
        return String(bytes / 1024.0 / 1024.0 / 1024.0) + " GB";
}

#endif // MYCFG_h
