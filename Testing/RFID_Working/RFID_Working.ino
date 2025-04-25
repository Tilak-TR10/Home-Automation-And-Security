/*
PINOUT:
RC522 MODULE    Uno/Nano     MEGA
SDA             D10          D9
SCK             D13          D52
MOSI            D11          D51
MISO            D12          D50
IRQ             N/A          N/A
GND             GND          GND
RST             D9           D8
3.3V            3.3V         3.3V
*/
/* Include the standard Arduino SPI library */
#include <SPI.h>
/* Include the RFID library */
#include <RFID.h>


/* Define the DIO used for the SDA (SS) and RST (reset) pins. */
#define SDA_DIO 10
#define RESET_DIO 8
/* Create an instance of the RFID library */
RFID RC522(SDA_DIO, RESET_DIO); 

void setup()
{ 
  Serial.begin(9600);
  /* Enable the SPI interface */
  SPI.begin(); 
  /* Initialise the RFID reader */
  RC522.init();
  Serial.println("RFID");

}

void loop()
{
  /* Has a card been detected? */
  if (RC522.isCard()){
    String cardUID = "";  // Variable to store the UID as a hex string
    /* If so then get its serial number */
    RC522.readCardSerial();
    Serial.println("Card detected:");
    
    for(int i=0;i<5;i++){
       if (RC522.serNum[i] < 0x10) {
        cardUID += "0";  // Ensure two-digit format
       }
       //Serial.print(RC522.serNum[i],DEC);
       //Serial.print(RC522.serNum[i],HEX); //to print card detail in Hexa Decimal format
       cardUID += String(RC522.serNum[i], HEX);  // Convert to hex
    }
    cardUID.toUpperCase();
    //Serial.println(cardUID.substring(0));  // Print the stored UID
    if (cardUID.substring(0) == "A7D39100E5") {  // Example authorized card UID    
      Serial.println("Access granted");
    } else {
      Serial.println("Access denied");
    }
  }
  delay(100);
}
