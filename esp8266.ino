#include "MyWifi.h"
#include <OneWire.h>
#include <DallasTemperature.h>

#define UNIQUE_ID 00001 // Number to differenciate measure points

#define WARNING_PIN D0  // Led pin number
#define SENSOR_PIN A0   // Sensor pin number

// WiFi
/*char* WIFI_SSID = "ieeehotspot";
  char* WIFI_PASSWORD = "Jk638td9og35";*/
char* WIFI_SSID = "Vodafone-F543D9";
char* WIFI_PASSWORD = "038E3744D1";

int TEMP_MAX = 22;
int TEMP_MIN = 30;

ESP8266WebServer server(80);  // Define port 80 for the web server port

OneWire oneWire(SENSOR_PIN); // Setup a oneWire instance to communicate with any OneWire devices

DallasTemperature sensors(&oneWire);  // Pass our oneWire reference to Dallas Temperature.

int temp;

void respond() {
  char data[10]; // variable to hold the temperature as string
  sprintf(data, "%d,%d", UNIQUE_ID, sensors.getTempCByIndex(0));  // Arranges data in a coma separated string for easy data extraction
  server.send(200, "text/plain", data); // Send data as response
}

void setup() {
  Serial.begin(115200); // Initializes serial with baud rate as a parameter
  connectWifi(WIFI_SSID, WIFI_PASSWORD); // Tries to connect to a wifi network with credentials as parameters
  Serial.println("");
  Serial.println(WiFi.localIP());   // Prints internal IP on serial TODO: Make it print on the LCD
  pinMode(SENSOR_PIN, INPUT);       // Defines sensor pin as being an input
  pinMode(WARNING_PIN, OUTPUT);     // Defines sensor pin as being an input
  digitalWrite(WARNING_PIN, LOW);   // Set the starting state of the led as turned off
  server.on("/", respond);          // Listen for HTTP/GET requests to respond appropriately
  server.begin();                   // Start web server
}

void loop() {
  server.handleClient();  // Makes sure to reconnect if wifi fails and other stuff

  sensors.requestTemperatures(); // Send the command to get temperatures

  temp = sensors.getTempCByIndex(0);        // Gets the current temperature
  if (temp > TEMP_MAX || temp < TEMP_MIN) { //Checks if the temperature is within the defined limits
    digitalWrite(WARNING_PIN, HIGH);  // If the temperature is out of limits lights up the warning led
  } else {
    digitalWrite(WARNING_PIN, LOW);   // If the temperature is within the limits turns off the warning led
  }
}
