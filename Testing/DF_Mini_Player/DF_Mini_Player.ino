#include <SoftwareSerial.h>
#include "DFRobotDFPlayerMini.h"

static const uint8_t PIN_MP3_TX = 26; // Connects to module's RX 
static const uint8_t PIN_MP3_RX = 27; // Connects to module's TX 

SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);

// Create the Player object
DFRobotDFPlayerMini player;

void setup() {
  Serial.begin(9600);                  //Init USB serial port for debugging
  softwareSerial.begin(9600);          //Init serial port for DFPlayer Mini
  if (player.begin(softwareSerial)) {
    player.volume(30);                // Set volume to maximum (0 to 30).
    player.play(1);                   // Play the first MP3 file on the SD card
  } else {
    Serial.println("Connecting to DFPlayer Mini failed!");
  }
}

void loop() {
}
