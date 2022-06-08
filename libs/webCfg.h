#if !defined(WEBCFG_h)
#define WEBCFG_h

#include "myCfg.h"

constexpr char *apssid = "TONY_NAS";
constexpr char *appsk = "mynas080622";
constexpr char *dnsDom = "www.nas.com";
constexpr auto DNS_PORT = 53;

bool needReboot(false);
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

String processor(const String &var)
{
    if (var == "TOTALSD")
        return humanReadableSize(SD.size64()); // humanReadableSize(info_64.totalBytes);
    return String();
}

String listFiles()
{
    File root = SD.open("/");
    File foundfile = root.openNextFile();
    String returnText{"<table><tr><th align='left'>Name</th><th align='left'>Size</th><th></th><th></th></tr>"};
    while (foundfile)
    {
        returnText += "<tr align='left'><td>" + String(foundfile.name()) + "</td><td>" + humanReadableSize(foundfile.size()) + "</td>";
        returnText += "<td><button id=\"down\" onclick=\"downloadDeleteButton(\'" + String(foundfile.name()) + "\', \'download\')\">Download</button>";
        returnText += "<td><button id=\"del\" onclick=\"downloadDeleteButton(\'" + String(foundfile.name()) + "\', \'delete\')\">Delete</button></tr>";
        foundfile = root.openNextFile();
    }
    returnText += "</table>";
    root.close();
    foundfile.close();
    return returnText;
}

void handleUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
    if (!index) // open the file on first call and store the file handle in the request object
    {
        request->_tempFile = SD.open("/" + filename, "w");
        stage = START;
    }
    if (len) // stream the incoming chunk to the opened file
    {
        request->_tempFile.write(data, len);
        stage = ING;
    }

    if (final)
    { // close the file handle as the upload is now done
        request->_tempFile.close();
        request->redirect("/");
        stage = IDLE;
    }
}

void handleFile(AsyncWebServerRequest *request)
{
    if (request->hasParam("name") && request->hasParam("action"))
    {
        String fileName = request->getParam("name")->value();
        String fileAction = request->getParam("action")->value();

        if (!SD.exists(fileName))
            request->send(400, "text/plain", "ERROR: file does not exist");
        else
        {
            if (fileAction == "download")
                request->send(SDFS, fileName, String(), true);
            else if (fileAction == "delete")
            {
                SD.remove(fileName);
                request->send(200, "text/plain", "Deleted File: " + fileName);
            }
            else
                request->send(400, "text/plain", "ERROR: invalid action param supplied");
        }
    }
    else
        request->send(400, "text/plain", "ERROR: name and action params required");
}

void handleNotFound(AsyncWebServerRequest *request)
{
    String path{request->url()};
    String mime{};
    if (path.endsWith("/"))
    {
        path += "index.html";
        request->send(SDFS, "/index.html", "text/html", false, processor);
    }
    else if (SD.exists(path))
    {
        if (path.endsWith(".html") || path.endsWith(".htm"))
            mime = "text/html";
        else if (path.endsWith(".css"))
            mime = "text/css";
        else if (path.endsWith(".js"))
            mime = "text/javascript";
        else if (path.endsWith(".png"))
            mime = "image/png";
        else if (path.endsWith(".gif"))
            mime = "image/gif";
        else if (path.endsWith(".jpg") || path.endsWith(".jpeg"))
            mime = "image/jpeg";
        else if (path.endsWith(".ico"))
            mime = "image/x-icon";
        else if (path.endsWith(".xml"))
            mime = "text/xml";
        else if (path.endsWith(".mp4"))
            mime = "video/mp4";
        else if (path.endsWith(".pdf"))
            mime = "application/pdf";

        request->send(SDFS, path, mime);
    }
    else
    {
        String message = "URI: " + request->url() + "\nMethod: " + request->methodToString() + "\nArguments: " + String(request->args()) + "\n";
        for (uint8_t i = 0; i < request->args(); i++)
            message += " NAME:" + request->argName(i) + "\n VALUE:" + request->arg(i) + "\n";
        request->send(404, "text/plain", message);
    }
}
#endif // WEBCFG_h
