#include "Arduino.h"
#include <WiFi.h>
#include "time.h"
#include "esp_sntp.h"
#include "Secure.h"
#include "CronAlarms.h"
#include "TimeHandler.h"

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
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi!");

  // Initialize TimeHandler
  TimeHandler::init("America/New_York");

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