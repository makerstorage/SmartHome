/*
  Nuri Erginer
  www.makerstorage.com
  credits: Omer Siar Baysal

*/

#include <SPI.h>        // RC522 Module uses SPI protocol
#include <MFRC522.h>	// Library for Mifare RC522 Devices


// #include <Servo.h>

#define NOTE_C4  262
#define NOTE_C5  523
#define NOTE_C6  1047
#define NOTE_G4  392
#define NOTE_E5  659
#define NOTE_G5  784

#define sAudioPin 4


//#define COMMON_ANODE

#ifdef COMMON_ANODE
#define LED_ON LOW
#define LED_OFF HIGH
#else
#define LED_ON HIGH
#define LED_OFF LOW
#endif

#define redLed 6		// Set Led Pins
#define greenLed 7
#define blueLed 5


#define buzzer 4      // Set Relay Pin


int successRead;    // Variable integer to keep if we have Successful Read from Reader

byte readCard[4];		// Stores scanned ID read from RFID Module


/*
	We need to define MFRC522's pins and create instance
	Pin layout should be as follows (on Arduino Uno):
	MOSI: Pin 11 / ICSP-4
	MISO: Pin 12 / ICSP-1
	SCK : Pin 13 / ICSP-3
	SS : Pin 10 (Configurable)
	RST : Pin 9 (Configurable)
	look MFRC522 Library for
	other Arduinos' pin configuration 
 */

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);	// Create MFRC522 instance.

///////////////////////////////////////// Setup ///////////////////////////////////
void setup() {
  //Arduino Pin Configuration
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(blueLed, OUTPUT);
  
 
  digitalWrite(redLed, LED_OFF);	// Make sure led is off
  digitalWrite(greenLed, LED_OFF);	// Make sure led is off
  digitalWrite(blueLed, LED_OFF);	// Make sure led is off

  //Protocol Configuration
  Serial.begin(115200);	 // Initialize serial communications with PC
  SPI.begin();           // MFRC522 Hardware uses SPI protocol
  mfrc522.PCD_Init();    // Initialize MFRC522 Hardware
  
  //If you set Antenna Gain to Max it will increase reading distance
  mfrc522.PCD_SetAntennaGain(mfrc522.RxGain_max);
  

}


///////////////////////////////////////// Main Loop ///////////////////////////////////
void loop () {
  do {
    successRead = getID(); 	// sets successRead to 1 when we get read from reader otherwise 0

    normalModeOn(); 		// Normal mode, blue Power LED is on, all others are off
    
  }
  while (!successRead); 	//the program will not go further while you not get a successful read
  
      digitalWrite(blueLed, LED_OFF);   // Turn off blue LED
      digitalWrite(redLed, LED_OFF);  // Turn off red LED
      digitalWrite(greenLed, LED_ON);   // Turn on green LED
      successTone();
      delay(1000);    



// Serial.println("Hello123");

// delay(3000);
            
}





///////////////////////////////////////// Get PICC's UID ///////////////////////////////////
int getID() {
  // Getting ready for Reading PICCs
  if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
    return 0;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {   //Since a PICC placed get Serial and continue
    return 0;
  }
  // There are Mifare PICCs which have 4 byte or 7 byte UID care if you use 7 byte PICC
  // I think we should assume every PICC as they have 4 byte UID
  // Until we support 7 byte PICCs
//  Serial.println(F("Scanned PICC's UID:"));
  for (int i = 0; i < 4; i++) {  //
    readCard[i] = mfrc522.uid.uidByte[i];
    Serial.print(readCard[i], HEX);
  }
  Serial.println("");
  mfrc522.PICC_HaltA(); // Stop reading
  return 1;
}




//////////////////////////////////////// Normal Mode Led  ///////////////////////////////////
void normalModeOn () {
  digitalWrite(blueLed, LED_ON); 	// Blue LED ON and ready to read card
  digitalWrite(redLed, LED_OFF); 	// Make sure Red LED is off
  digitalWrite(greenLed, LED_OFF); 	// Make sure Green LED is off
 
}


void myMasterTone() {
          tone(sAudioPin,NOTE_C5);
          delay(150);
          noTone(sAudioPin);
          tone(sAudioPin,NOTE_C5);
          delay(150);
          tone(sAudioPin,NOTE_C6);
          delay(400);
          noTone(sAudioPin);
}

void rejectTone(){
          tone(sAudioPin,NOTE_G4);
          delay(250);
          tone(sAudioPin,NOTE_C4);
          delay(500);
          noTone(sAudioPin);
}

void successTone(){
         
          tone(sAudioPin,NOTE_C5);
          delay(150);
          noTone(sAudioPin);
          tone(sAudioPin,NOTE_E5);
          delay(150);
          tone(sAudioPin,NOTE_G5);
          delay(400);
          noTone(sAudioPin);
}          

