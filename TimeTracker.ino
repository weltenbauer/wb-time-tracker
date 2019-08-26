
#include <SPI.h>
#include <MFRC522.h>

#include "config.h"
#include "src/connection.h"
#include "src/clockify.h"
#include "src/util.h"

//-----------------------------------------------------------------------------------------------

MFRC522 mfrc522(RC522_SS_PIN, RC522_RST_PIN);

//-----------------------------------------------------------------------------------------------

void setup() {

  // Start serial
  Serial.begin(9600);
  while (!Serial){
    continue;
  }

  // Start application
  msgPrintln("Welcome to " APP_NAME);

  // State
  msgPrintln("\nInit Application\n-------------------------------------");

  // Init RC522
  SPI.begin();
  mfrc522.PCD_Init();
  mfrc522.PCD_DumpVersionToSerial();

  // Connect to network
  connect();

  // Init data
  initTags();

  // State
  msgPrintln("\nInit done. Waiting for cards!\n-------------------------------------");
}

//-----------------------------------------------------------------------------------------------

void loop() {

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if (!mfrc522.PICC_IsNewCardPresent()) return;

  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) return;

  // Get Card-ID
  String cardId;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    cardId.concat(mfrc522.uid.uidByte[i]);
  }

  // Get User-ID for card
  String userId = getUserId(cardId);

  // Check if User-ID exists
  if(userId.length() > 0){
    
    // State
    msgPrint("Stop timer for: ");
    msgPrint(getUserName(cardId));
    msgPrint(" - ");
    msgPrintln(userId);

    // Try to stop timer. If this fails, start a new timer
    if(stopTimer(userId) == 404){

      // State
      msgPrintln("-------------------------------------");
      msgPrint("Start timer for: ");
      msgPrint(getUserName(cardId));
      msgPrint(" - ");
      msgPrintln(userId);

      // Start timer for user
      startTimer(userId);
    }
  }
  else{

    // Print error when User for Card-ID doesnt exists
    msgPrint("User for Card-ID ");
    msgPrint(cardId);
    msgPrintln(" unknown.");
  }

  // Move on
  msgPrintln("-------------------------------------");
}
