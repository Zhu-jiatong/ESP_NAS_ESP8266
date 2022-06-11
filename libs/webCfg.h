#if !defined(WEBCFG_h)
#define WEBCFG_h

#include "myCfg.h"

constexpr char *apssid = "TONY_NAS";
constexpr char *dnsDom = "www.nas.com";
constexpr auto DNS_PORT = 53;

bool needReboot(false);
DNSServer APdnsServer, STAdnsServer;
AsyncWebServer server(80);

String processor(const String &var)
{
    if (var == "TOTALSD")
        return SDsize; // humanReadableSize(info_64.totalBytes);
    return String();
}

String listFiles()
{
    Dir root = SDFS.openDir("/");
    String returnText{"<table><tr><th align='left'>Name</th><th align='left'>Size</th><th></th><th></th></tr>"};
    while (root.next())
    {
        returnText += "<tr align='left'><td>" + String(root.fileName()) + "</td><td>" + humanReadableSize(root.fileSize()) + "</td>";
        returnText += "<td><button id=\"prev\" onclick=\"showPreview(\'" + String(root.fileName()) + "\')\">Preview</button></td>";
        returnText += "<td><button id=\"down\" onclick=\"downloadDeleteButton(\'" + String(root.fileName()) + "\', \'download\')\">Download</button></td>";
        returnText += "<td><button id=\"del\" onclick=\"downloadDeleteButton(\'" + String(root.fileName()) + "\', \'delete\')\">Delete</button></td>";
    }
    returnText += "</tr></table>";
    return returnText;
}

void handleUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
    if (!index) // open the file on first call and store the file handle in the request object
        request->_tempFile = SD.open("/" + filename, "w");
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

void handleSTA(AsyncWebServerRequest *request)
{
    if (request->hasParam("sta_ssid") && request->hasParam("sta_psk"))
    {
        String message;
        if (!request->getParam("sta_ssid")->value().length())
        {
            WiFi.disconnect();
            return;
        }

        switch (WiFi.begin(request->getParam("sta_ssid")->value(), request->getParam("sta_psk")->value()))
        {
        case WL_CONNECTED:
            message = "WL_CONNECTED";
            break;
        case WL_NO_SSID_AVAIL:
            message = "WL_NO_SSID_AVAIL";
            break;
        case WL_CONNECT_FAILED:
            message = "WL_CONNECT_FAILED";
            break;
        case WL_WRONG_PASSWORD:
            message = "WL_WRONG_PASSWORD";
            break;
        case WL_IDLE_STATUS:
            message = "WL_IDLE_STATUS";
        default:
            message = "WL_DISCONNECTED";
            break;
        }
        request->send(200, "text/plain", message);
    }
    else
        request->send(400, "text/plain", "ERROR: name and action params required");
}
#endif // WEBCFG_h
