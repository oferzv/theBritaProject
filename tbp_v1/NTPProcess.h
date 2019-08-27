#include <NTPClient.h>
#include <WiFiUdp.h>
const long utcOffsetInSeconds = 7200;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void NTPSetup() { 
  timeClient.begin();
}

