#include <SPI.h>
#include <RFID.h>
#include <SoftwareSerial.h>

// Pin definitions
const int gasSensorPin = 6;       //Gas Sensor 
const int gasRelayPin = 2;        // RELAY to control Exhaust Fan
const int pirSensorPin = 7;       // PIR Sensor Pin
const int rainSensorPin = 9;      // Rain Sensor Pin
const int armed_R_LED = A4;       //Armed LED
const int disarmed_G_LED = A5;    // Disarmed LED

// GSM Module Pins
const int rxPin = 3;  // Arduino D3 Goes to TXD (GSM Side)
const int txPin = 5;  // Arduino D5 Goes to RXD (GSM Side)

#define RFID_TIMEOUT 30000 // 30 seconds in milliseconds
bool door = false;    // false = LOCK   FLAG Register
bool stopCall = false;

/* Define the DIO used for the SDA (SS) and RST (reset) pins. */
#define SDA_DIO 10
#define RESET_DIO 8
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define IR1 A3  // Entrance 1
#define IR2 A2  // Entrance 2
#define IR3 A1  // Exit 1
#define IR4 A0  // Exit 2

int peopleCount = 0;
int sequence[4] = {0, 0, 0, 0}; // To track sensor activations
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Create an instance of the RFID library */
RFID RC522(SDA_DIO, RESET_DIO);

SoftwareSerial gsmSerial(rxPin, txPin);  // Initialize SoftwareSerial for GSM module

String phoneNumber = "+919328048342";  // Replace with your actual phone number

/*----------------------------------------------------------------------GAS Sensor--------------------------------------------------------------------------------*/
// Function to control the gas relay
void controlGasRelay() {                                                          //100% Working
  int gasValue = digitalRead(gasSensorPin);   //Serial.println(gasValue);
  if (gasValue == HIGH) {  
    digitalWrite(gasRelayPin, HIGH);           // Turn ON relay
    sendSMS("Gas detected!!!");               // Send SMS alert
    makeCall();
  } else if(gasValue == LOW) {
    digitalWrite(gasRelayPin, LOW);          // Turn OFF relay
  }
}

/*----------------------------------------------------------------------Rain Sensor------------------------------------------------------------------------------*/
// Function to detect rain
void detectRain() {                                                                //100% Working
  int rainValue = digitalRead(rainSensorPin);
  if (rainValue == LOW) {  // LOW means rain detected    //Serial.println("Rain detected!");
    sendSMS("It's it's Raining!!");
  } else if (rainValue == HIGH) {}                                               //Serial.println("No rain detected.");
}

/*----------------------------------------------------------------------PIR Sensor--------------------------------------------------------------------------------*/
// Function to detect motion with PIR sensor                                      //100% Working
void detectMotion() {
  int motionValue = digitalRead(pirSensorPin);
  if (motionValue == HIGH) {                    //Serial.println("Motion detected!");
    sendSMS("Intruder!!!"); // Send SMS alert
  } else if (motionValue == LOW) {}                                     //Serial.println("No motion detected.");
}

/*----------------------------------------------------------------------RFID------------------------------------------------------------------------------------*/
void readRFID() {        // Function to read RFID card                      //100% Working
  if (RC522.isCard()){
    String cardUID = "";  // Variable to store the UID as a hex string
    /* If so then get its serial number */
    RC522.readCardSerial();
    Serial.println("Card detected:");
    
    for(int i=0;i<5;i++){
       if (RC522.serNum[i] < 0x10) {
        cardUID += "0";  // Ensure two-digit format
       }
       //Serial.print(RC522.serNum[i],DEC);  //Serial.print(RC522.serNum[i],HEX);
       cardUID += String(RC522.serNum[i], HEX);  // Convert to hex
    }
    cardUID.toUpperCase();  //Serial.println(cardUID.substring(0));  // Print the stored UID
    if (cardUID.substring(0) == "A7D39100E5" && door == false) {  // Example authorized card UID  // Check valid card or not
      Serial.println("Access Granted, Door Open");
      door = true;
      analogWrite(armed_R_LED, 0);
      analogWrite(disarmed_G_LED, 200);
      delay(500);
    } else if(cardUID.substring(0) == "A7D39100E5" && door == true) {
      Serial.println("Access Granted, Door Closed");
      door = false;
      analogWrite(armed_R_LED, 200);
      analogWrite(disarmed_G_LED, 0);
      delay(500);
    } else {
      Serial.println("Access Denied");
    }
  }
}

/*----------------------------------------------------------------------IR Array--------------------------------------------------------------------------------*/

void irdetect(){
  int s1 = digitalRead(IR1);
  int s2 = digitalRead(IR2);
  int s3 = digitalRead(IR3);
  int s4 = digitalRead(IR4);

  // Update sequence tracking
  if (s1 == HIGH && sequence[0] == 0) {
  sequence[0] = 1;
  Serial.println(sequence[0]);
  }
  
  if (s2 == HIGH && sequence[0] == 1 && sequence[1] == 0){
  sequence[1] = 2;
  Serial.println(sequence[1]);
  }
  
  if (s3 == HIGH && sequence[1] == 2 && sequence[2] == 0){
  sequence[2] = 3;
  Serial.println(sequence[2]);
  }
  
  if (s4 == HIGH && sequence[2] == 3 && sequence[3] == 0){
    sequence[3] = 4;
    Serial.println(sequence[3]);
    peopleCount++; // Person entered
    Serial.println("Person Entered - Count: " + String(peopleCount));
    resetSequence(); // Reset tracking
    analogWrite(armed_R_LED, 0);
    delay(100);
    analogWrite(armed_R_LED, 200);
    delay(100);
    analogWrite(armed_R_LED, 0);
    delay(100);
    checkRFID();
  }
}  

void resetSequence() {
  for (int i = 0; i < 4; i++) {
    sequence[i] = 0;
  }
  delay(1000);  // Avoid multiple detections
}

void checkRFID() {
    unsigned long startTime = millis();

    Serial.println("Waiting for RFID scan...");

    while (millis() - startTime < RFID_TIMEOUT) {
        readRFID(); // Call RFID function
        if (door) { // If door was unlocked by RFID, exit
            Serial.println("RFID detected, exiting check.");
            peopleCount = 0;
            return;
        }
        analogWrite(armed_R_LED, 0);
        delay(100);
        analogWrite(armed_R_LED, 200);
        delay(100);
    }
    Serial.println("No RFID detected in 30 seconds, sending SMS...");
    sendSMS("Hello, Someone at door");
    delay(10000);
    makeCall();
    peopleCount = 0;            
}

/*----------------------------------------------------------------------GSM------------------------------------------------------------------------------------*/
// Function to send SMS using GSM module
void sendSMS(String message) {                                   //100% Working
    gsmSerial.println("AT+CMGF=1");  // Set SMS mode to text
    delay(1000);
    gsmSerial.print("AT+CMGS=\"");  
    gsmSerial.print(phoneNumber);
    gsmSerial.println("\"");
    delay(1000);
    gsmSerial.print(message);  
    delay(500);
    gsmSerial.write(26);  // Send Ctrl+Z to end message
    delay(5000);
    Serial.println("SMS Sent!");
}

void makeCall() {                                                //100% Working
  gsmSerial.println("ATD" + phoneNumber + ";"); // Dial the number
  delay(10000); // Wait for 10 seconds before hanging up (adjust as needed)
  gsmSerial.println("ATH"); // Hang up the call
}

/*----------------------------------------------------------------------SETUP---------------------------------------------------------------------------------*/
void setup() {
  Serial.begin(9600);
  gsmSerial.begin(9600);  // Initialize GSM module serial

  SPI.begin();    // Enable the SPI interface 
  RC522.init();   // Initialise the RFID reader 

  Serial.println("System Initialized");

  pinMode(gasSensorPin, INPUT);
  pinMode(pirSensorPin, INPUT);  // PIR Sensor as Input
  pinMode(rainSensorPin, INPUT);  // Rain Sensor as Input
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);
  pinMode(IR4, INPUT);
  pinMode(gasRelayPin, OUTPUT);
  pinMode(armed_R_LED, OUTPUT);
  pinMode(disarmed_G_LED, OUTPUT);

  digitalWrite(gasRelayPin, LOW);  // Ensure relay is OFF initially
  analogWrite(armed_R_LED, 200);
  analogWrite(disarmed_G_LED, 0);
}

/*----------------------------------------------------------------------LOOP----------------------------------------------------------------------------------*/

void loop() {
  readRFID();                //Done                                    
  if (door == true) {                  //If Door is Unocked 
     controlGasRelay();    //Done
     detectRain();         //Done                                        //100% Working
  } else if(door == false) {           //Else Door is Locked
     detectMotion();                                                       //100% Working
     irdetect();
     controlGasRelay();    //Done
     detectRain();         //Done
   }
  delay(100);  
}