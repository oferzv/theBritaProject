/*
   ESP8266 NodeMCU AJAX Demo
   Updates and Gets data from webpage without page refresh
   https://circuits4you.com
*/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include "index.h" //Our HTML webpage contents with javascripts

#define LED 2  //On board LED

//SSID and Password of your WiFi router
const char* ssid = "YG10";
const char* password = "ABAeven2";

ESP8266WebServer server(80); //Server on port 80


#include <Servo.h>

Servo coverservo;  // create servo object
#define COVERSERVO_OPEN 0
#define COVERSERVO_CLOSE 90



#include "HX711.h"


// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = D2;
const int LOADCELL_SCK_PIN = D1;

float calibration_factor = 0;
HX711 scale;
float weight;

#define MIN_WEIGHT 750
#define FULL_WEIGHT 1600

#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 8

#define DATA_PIN D5

// Define the array of leds
CRGB leds[NUM_LEDS];

#define BRIGHTNESS 96


#define SS_IDLE 0
#define SS_PITCHER_PLACED 1
#define SS_FILL_PITCHER 2
#define SS_FULL_PITCHER 3

byte systemState;
unsigned long systemStateMillis;
String stateName = "";
//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void handleRoot() {
  String s = MAIN_page; //Read HTML contents
  server.send(200, "text/html", s); //Send web page
}


void handleDataRead() {
  String adcValue = String(weight, 2);
  server.send(200, "text/plane", stateName + "," + adcValue);
}


void handleLED() {
  String ledState = "OFF";
  String t_state = server.arg("LEDstate"); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
  Serial.println(t_state);
  if (t_state == "1")
  {
    digitalWrite(LED, LOW); //LED ON
    ledState = "ON"; //Feedback parameter
    moveServo(COVERSERVO_CLOSE);
  }
  else
  {
    digitalWrite(LED, HIGH); //LED OFF
    ledState = "OFF"; //Feedback parameter
    moveServo(COVERSERVO_OPEN);
  }

  server.send(200, "text/plane", ledState); //Send web page
}
//==============================================================
//                  SETUP
//==============================================================
void setup(void) {
  Serial.begin(115200);

  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  //Onboard LED port Direction output
  pinMode(LED, OUTPUT);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP

  server.on("/", handleRoot);      //Which routine to handle at root location. This is display page
  server.on("/setLED", handleLED);
  server.on("/readData", handleDataRead);
  server.begin();                  //Start server
  Serial.println("HTTP server started");

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  scale.set_scale(190.f);
  scale.tare();

  moveServo(COVERSERVO_CLOSE);

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  setSystemState(SS_IDLE);
}
//==============================================================
//                     LOOP
//==============================================================
void loop(void) {
  weight = scale.get_units();
  switch (systemState) {
    case SS_IDLE:
      stateName = "No Pitcher";
      if (weight > MIN_WEIGHT)
        setSystemState(SS_PITCHER_PLACED);
      break;
    case SS_PITCHER_PLACED:
      stateName = "Pitcher Placed";
      if (systemStateMillis + 1000 < millis()) {
        if (weight > FULL_WEIGHT)
          setSystemState(SS_FULL_PITCHER);
        else if (weight < MIN_WEIGHT)
          setSystemState(SS_IDLE);
        else
          setSystemState(SS_FILL_PITCHER);
      }
      break;
    case SS_FILL_PITCHER:
      stateName = "Filling Pitcher";
      if (weight > FULL_WEIGHT) {
        Serial.println("CLOSE WATRER");
        setSystemState(SS_FULL_PITCHER);
      } else if (weight < MIN_WEIGHT) {
        Serial.println("CLOSE WATRER");
        setSystemState(SS_IDLE);
      }
      break;
    case SS_FULL_PITCHER:
      stateName = "Full Pitcher";
      if (weight < MIN_WEIGHT)
        setSystemState(SS_IDLE);
      break;
  }
  server.handleClient();          //Handle client requests
}

void setSystemState(byte newState) {
  switch (newState) {
    case SS_IDLE:
      setLedColor(CRGB::Blue);
      moveServo(COVERSERVO_CLOSE);
      break;
    case SS_PITCHER_PLACED:
      setLedColor(CRGB::Orange);
      break;
    case SS_FILL_PITCHER:
      setLedColor(CRGB::Red);
      moveServo(COVERSERVO_OPEN);
      Serial.println("START WATER");
      break;
    case SS_FULL_PITCHER:
      setLedColor(CRGB::Green);
      moveServo(COVERSERVO_CLOSE);
      break;
  }

  systemState = newState;
  systemStateMillis = millis();
}

void setLedColor(CRGB color) {
  for (byte i = 0; i < NUM_LEDS; i++) {
    leds[i] = color;
  }
  FastLED.show();
}

void moveServo(byte value) {
  coverservo.attach(D6);  // attaches the servo on GIO2 to the servo object
  coverservo.write(value);
  delay(1500);
  coverservo.detach();
}