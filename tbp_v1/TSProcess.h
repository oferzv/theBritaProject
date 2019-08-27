#include "ThingSpeak.h"
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

// Initialize our values
int number1 = 0;
String myStatus = "";

void TSSetup() {
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

boolean TSUpdate(String systemState, int qty) {

  // set the fields with the values
  ThingSpeak.setField(1, qty);
  // ThingSpeak.setField(2, number2);
  // ThingSpeak.setField(3, number3);
  //  ThingSpeak.setField(4, number4);

  // set the status
  ThingSpeak.setStatus(systemState);

  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (x == 200) {
    Serial.println("Channel update successful.");
    return true;
  }
  else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
    return false;
  } //end if 


}
