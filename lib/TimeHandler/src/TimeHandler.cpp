#ifdef ENABLE_TIME_HANDLER

#include "TimeHandler.h"

static bool isTimeSynced = false;

// NTP servers
static const char *ntpServer1 = "pool.ntp.org";
static const char *ntpServer2 = "time.nist.gov";

// Dynamically set offsets
static long gmtOffset_sec = 0;
static int daylightOffset_sec = 0;

// Default region hardcoded
const char* defaultRegion = "Etc/Zulu";

void TimeHandler::init(const char* timezone)
{
    Serial.println("[INFO] TimeHandler initialized");

    if (timezone && strlen(timezone) > 0) {
        defaultRegion = timezone;
        Serial.printf("[INFO] TimeHandler: Using timezone from parameter: %s\n", defaultRegion);
    }else {
        Serial.printf("[WARN] TimeHandler: No timezone set. Using default: %s\n", defaultRegion);
    }

    Serial.printf("[INFO] TimeHandler: Using timezone: %s\n", defaultRegion);

    const char *posix_str = tz_db_get_posix_str(defaultRegion);
    if (!posix_str)
    {
        Serial.printf("[ERROR] TimeHandler: Failed to find POSIX string for region %s\n", defaultRegion);
        return;
    }
    Serial.printf("[INFO] TimeHandler: Found POSIX string: %s\n", posix_str);

    if (!parsePosixOffsets(posix_str, &gmtOffset_sec, &daylightOffset_sec))
    {
        Serial.printf("[ERROR] TimeHandler: Failed to parse offsets from POSIX string: %s\n", posix_str);
        return;
    }
    Serial.printf("[INFO] TimeHandler: Parsed offsets - gmtOffset_sec: %ld, daylightOffset_sec: %d\n", gmtOffset_sec, daylightOffset_sec);

    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);
    syncTime();
}

void TimeHandler::loop()
{
    if (!isTimeSynced)
    {
        syncTime();
        delay(60000);
    }
}

void TimeHandler::syncTime()
{
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        Serial.println("[WARN] TimeHandler: Failed to obtain time");
        return;
    }

    Serial.printf("[INFO] TimeHandler: Time successfully synchronized. Current local time: %s\n", formatDateTime("%Y-%m-%d %I:%M:%S %p"));
    isTimeSynced = true;
}

bool TimeHandler::parsePosixOffsets(const char *posixStr, long *gmtOffset, int *daylightOffset)
{
    if (!posixStr || !gmtOffset || !daylightOffset)
    {
        return false;
    }

    const char *gmtStart = strchr(posixStr, 'T');
    if (!gmtStart)
    {
        return false;
    }
    *gmtOffset = -(atoi(gmtStart + 1) * 3600);

    const char *daylightStart = strchr(posixStr, ',');
    if (daylightStart)
    {
        *daylightOffset = 3600;
    }
    else
    {
        *daylightOffset = 0;
    }

    return true;
}

long TimeHandler::getLinuxTime()
{
    return time(nullptr);
}

const char* TimeHandler::formatDateTime(const char* format) {
    static char buffer[64];
    struct tm timeinfo;
    time_t now = time(nullptr);

    if (!getLocalTime(&timeinfo)) {
        return "Time not available";
    }

    strftime(buffer, sizeof(buffer), format, &timeinfo);
    return buffer;
}

void TimeHandler::logAllDateTimeFormats()
{
    Serial.printf("[INFO] Full Date-Time (12-hour): %s\n", formatDateTime("%Y-%m-%d %I:%M:%S %p"));
    Serial.printf("[INFO] Full Date-Time (24-hour): %s\n", formatDateTime("%Y-%m-%d %H:%M:%S"));
    Serial.printf("[INFO] Date Only: %s\n", formatDateTime("%Y-%m-%d"));
    Serial.printf("[INFO] Time Only (12-hour): %s\n", formatDateTime("%I:%M:%S %p"));
    Serial.printf("[INFO] Time Only (24-hour): %s\n", formatDateTime("%H:%M:%S"));
    Serial.printf("[INFO] Day of the Week: %s\n", formatDateTime("%A"));
    Serial.printf("[INFO] Month and Year: %s\n", formatDateTime("%B %Y"));
    Serial.printf("[INFO] Weekday, Month, Date: %s\n", formatDateTime("%A, %B %d"));
    Serial.printf("[INFO] Short Date-Time: %s\n", formatDateTime("%m/%d/%y %H:%M"));
    Serial.printf("[INFO] ISO 8601 Format: %s\n", formatDateTime("%Y-%m-%dT%H:%M:%S"));
    Serial.printf("[INFO] Linux Epoch Time: %ld\n", time(nullptr));
    Serial.printf("[INFO] Custom Timezone: %s\n", formatDateTime("%Z"));
}

#endif // ENABLE_TIME_HANDLER
