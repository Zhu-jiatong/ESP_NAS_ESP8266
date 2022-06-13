#if !defined(MYCFG_h)
#define MYCFG_h

#define sToMillis(sec) (sec * 1000)
#define milToS(mil) (mil / 1000)

bool isUpload{false};

String appsk;
bool hasSD{false};
String SDsize{};
unsigned long wakeTime{sToMillis(90)}, leftTime{};

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
