#include <WiFi.h>
#include "time.h"
#include "Secure.h"
#include "CronAlarms.h"

const char *time_zone = "EST5EDT,M3.2.0,M11.1.0"; // Time zone string
static const char *ntpServer1 = "pool.ntp.org";
static const char *ntpServer2 = "time.nist.gov";

void Test()
{
  Serial.println("Cron Timer!");
}

void setup()
{
  Serial.begin(115200);

  Serial.println("Starting setup...");

 // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi!");

  // Set the time zone and configure NTP
  configTzTime(time_zone, ntpServer1, ntpServer2);

  Serial.println("Waiting for time synchronization...");
  struct tm timeinfo;
  while (!getLocalTime(&timeinfo)) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nTime synchronized!");

  // Print the current time
  Serial.print("Current time: ");
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  
  //Cron.create("*/5 * * * * *", Test, false);   
  Cron.create("0 30 17 * * *", Test, false);

  Serial.println("Ending setup...");
}

void loop()
{
  time_t tnow = time(nullptr);
  
  //Serial.println(asctime(gmtime(&tnow)));
  Serial.println(asctime(localtime(&tnow)));

  Cron.delay(); // if the loop has nothing else to do, delay in ms should be provided as argument to ensure immediate trigger when the time is reached
  
  delay(1000);  // do other things... like waiting one second between clock display
}