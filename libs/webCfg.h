#if !defined(WEBCFG_h)
#define WEBCFG_h

#define DBG_OUTPUT_PORT Serial

#include <DNSServer.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>
#include <SD.h>

File uploadFile;
DNSServer dnsServer;
AsyncWebServer server(80);

String humanReadableSize(const size_t bytes)
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

String getMimeType(String &path)
{
    if (path.endsWith("/"))
        path += "index.html";

    if (path.endsWith(".html"))
        return "text/html";
    else if (path.endsWith(".css"))
        return "text/css";
    else if (path.endsWith(".js"))
        return "application/javascript";
    else if (path.endsWith(".png"))
        return "image/png";
    else if (path.endsWith(".gif"))
        return "image/gif";
    else if (path.endsWith(".jpg"))
        return "image/jpeg";
    else if (path.endsWith(".ico"))
        return "image/x-icon";
    else if (path.endsWith(".xml"))
        return "text/xml";
    else if (path.endsWith(".pdf"))
        return "application/pdf";
}

void getPage(AsyncWebServerRequest *request)
{
    String path{request->url()};
    String mimeType{getMimeType(path)};
    if (SD.exists(path))
        request->send(SDFS, path, mimeType);
    else
    {
        String message = "Invalid File\n\nURI: " + request->url() + "\nMethod: " + request->methodToString() + "\nArguments: " + String(request->args()) + "\n";
        for (uint8_t i = 0; i < request->args(); i++)
            message += " NAME:" + request->argName(i) + "\n VALUE:" + request->arg(i) + "\n";
        request->send(404, "text/plain", message);
    }
}

void listFiles(AsyncWebServerRequest *request)
{
    File root = SD.open("/");
    File foundfile = root.openNextFile();
    String returnText{"<table><tr><th align='left'>Name</th><th align='left'>Size</th><th></th><th></th></tr>"};
    while (foundfile)
    {
        returnText += "<tr align='left'><td>" + String(foundfile.name()) + "</td><td>" + humanReadableSize(foundfile.size()) + "</td>";
        returnText += "<td><button onclick=\"downloadDeleteButton(\'" + String(foundfile.name()) + "\', \'download\')\">Download</button>";
        returnText += "<td><button onclick=\"downloadDeleteButton(\'" + String(foundfile.name()) + "\', \'delete\')\">Delete</button></tr>";
        foundfile = root.openNextFile();
    }
    returnText += "</table>";
    root.close();
    foundfile.close();
    request->send(100, "text/html", returnText);
}

#endif // WEBCFG_h
