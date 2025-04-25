#include <WiFi.h>
#include <WebServer.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// WiFi Credentials
const char *ssid = "Home_Automation";
const char *password = "12345678";

//Reay Pins Defined
#define RELAY_1 19  // D19
#define RELAY_2 18  // D18
#define RELAY_3 17  // TX2
#define RELAY_4 4  // RX2  

#define ONE_WIRE_BUS 13   //Temperature PIN

OneWire oneWire(ONE_WIRE_BUS); // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensors(&oneWire); // Pass our oneWire reference to Dallas Temperature.

WebServer server(80); // Web Server

void handleRelay() {                                
  if (server.hasArg("State")) {
    String state = server.arg("State");
    if (state == "Switch_1_ON") {
      digitalWrite(RELAY_1, LOW);
      server.send(200, "text/plain", "Relay 1 ON");
    } else if (state == "Switch_1_OFF") {
      digitalWrite(RELAY_1, HIGH);
      server.send(200, "text/plain", "Relay 1 OFF");
    } else if (state == "Switch_2_ON") {
      digitalWrite(RELAY_2, LOW);
      server.send(200, "text/plain", "Relay 2 ON");
    } else if (state == "Switch_2_OFF") {
      digitalWrite(RELAY_2, HIGH);
      server.send(200, "text/plain", "Relay 2 OFF");
    } else if (state == "Switch_3_ON") {
      digitalWrite(RELAY_3, LOW);
      server.send(200, "text/plain", "Relay 3 ON");
    } else if (state == "Switch_3_OFF") {
      digitalWrite(RELAY_3, HIGH);
      server.send(200, "text/plain", "Relay 3 OFF");
    } else if (state == "Switch_4_ON") {
      digitalWrite(RELAY_4, LOW);
      server.send(200, "text/plain", "Relay 4 ON");
    } else if (state == "Switch_4_OFF") {
      digitalWrite(RELAY_4, HIGH);
      server.send(200, "text/plain", "Relay 4 OFF");
    }
  }else {
   server.send(400, "text/plain", "Missing parameters");
  }
}

String getTemperature() {                                             
    sensors.requestTemperatures(); // Send the command to get temperatures
    float tempC = sensors.getTempCByIndex(0);
    return String(tempC);
}

void handleRoot() {                     //100% Working
    server.send(200, "text/html", "<h1>ESP8266 Relay Control</h1>"); 
}

////////////////////////////////////SETUP///////////////////////////////////////////

void setup() {
    Serial.begin(115200);
    sensors.begin(); // Start up the library
////////////////////////////////////Wi-Fi//////////////////////////////////////////
    Serial.println("Setting up WiFi...");
    WiFi.softAP(ssid, password);                    //Connecting to Wifi using SSID and password
    
    while (WiFi.softAPgetStationNum() == 0) {       //In infinite Loop untill Wifi is connected 
        Serial.print(".");
        delay(2000);
    }
    Serial.println("\nWiFi AP started successfully.");   //Debug
    Serial.print("ESP32 IP Address: ");
    Serial.println(WiFi.softAPIP()); 
    delay(2000);
///////////////////////////////////////////////////////////////////////////////////
    
  // Set relay pins as outputs
    pinMode(RELAY_1, OUTPUT);
    pinMode(RELAY_2, OUTPUT);
    pinMode(RELAY_3, OUTPUT);
    pinMode(RELAY_4, OUTPUT);

  // Initially turn all relays OFF
    digitalWrite(RELAY_1, HIGH);
    digitalWrite(RELAY_2, HIGH);
    digitalWrite(RELAY_3, HIGH);
    digitalWrite(RELAY_4, HIGH);
    
    server.on("/", handleRoot);         
    server.on("/relay", handleRelay);  
    
    server.on("/gettemp", []() {
        String TemperatureC =  getTemperature();
        server.send(200, "text/plain", "Temperature: " + TemperatureC);
    });

    server.begin();
}


void loop() {
    server.handleClient();
}
