// MASTER
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Servo.h>
#include <SPI.h>

#define RS 4
#define EN 5
#define D4 6
#define D5 7
#define D6 8
#define D7 9

#define SERVO_PIN 3

#define DUMMY 255
#define PL1_BTN_SIG 10
#define PL2_BTN_SIG 20
#define TIMEOUT_SIG 128

#define PLAYER_BITMASK 0b00011111 

String player1="", player2="";

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
Servo motor;

int pl1_pts = 0, pl2_pts = 0;

bool gameStarted = false;

unsigned long timeout = 2000;

byte sendByte(byte data);
void initialState();
void printPts();
void playRound();

void setup() {
  Serial.begin(9600);

  SPI.begin();
  pinMode(SS, OUTPUT);
  digitalWrite(SS, HIGH);

  lcd.begin(16, 2);

  motor.attach(SERVO_PIN);
  motor.write(0);

  randomSeed(analogRead(0));
}

byte l = 0;

void loop() {
    initialState();
    while(gameStarted) {
        printPts();
        playRound();
        delay(500);
    }
}

byte sendByte(byte data) {
  digitalWrite(SS, LOW);
  byte slaveResponse = SPI.transfer(data);
  digitalWrite(SS, HIGH);
  if(data != DUMMY) {
    Serial.print("Sent byte to slave: ");
    Serial.print(data);
    Serial.println("");
  }
  
  return slaveResponse;
}

void initialState() {
  lcd.home();
  lcd.print("Press any button");
  lcd.setCursor(0, 1);
  lcd.print("to begin.");
  byte receivedFromSlave = sendByte(DUMMY);
  if ((receivedFromSlave & PLAYER_BITMASK) == PL1_BTN_SIG || (receivedFromSlave & PLAYER_BITMASK) == PL2_BTN_SIG) {
    gameStarted = true;
    sendByte(DUMMY);
  }
}

void printPts() {
  lcd.clear();
  lcd.home();
  lcd.print("Player 1: "); lcd.print(pl1_pts); lcd.print(" pts");
  lcd.setCursor(0, 1);
  lcd.print("Player 2: "); lcd.print(pl2_pts); lcd.print(" pts");
}

void playRound() {
  byte col1 = random(1, 4);
  byte col2 = random(1, 4);
  byte colsToSend = col1 | (col2 << 2);
  byte slaveResponse = sendByte(colsToSend);
  Serial.println("COLOR SENT TO SLAVE");
  unsigned long startTime = millis();

  do {
    slaveResponse = sendByte(DUMMY); 
    Serial.print("Slave response: ");
    Serial.print(slaveResponse);
    Serial.println("");
  }
 while ((slaveResponse == DUMMY || !(slaveResponse & 0b01100000)) && millis() - startTime < timeout);

  if (millis() - startTime >= timeout) {
    sendByte(TIMEOUT_SIG);
    Serial.println("TIMEOUT SIGNAL SENT!");
  } else {
    byte color = (slaveResponse & 0b01100000) >> 5;
    slaveResponse = slaveResponse & PLAYER_BITMASK;
    if(slaveResponse == PL1_BTN_SIG) {
      if(color == col1) {
        Serial.println("Player 1 pressed the correct button");
        pl1_pts += (millis() - startTime)/100;
      } else {
        Serial.println("Player 1 pressed the wrong button");
        pl1_pts -= 20;
        if(pl1_pts < 0) pl1_pts = 0;
      }
      sendByte(TIMEOUT_SIG);
    } else if(slaveResponse == PL2_BTN_SIG) {
      if(color == col2) {
        Serial.println("Player 2 pressed the correct button");
        pl2_pts += (millis() - startTime)/100;
      } else {
        Serial.println("Player 2 pressed the wrong button");
        pl2_pts -= 20;
        if(pl2_pts < 0) pl2_pts = 0;
      }
      sendByte(TIMEOUT_SIG);
    } else {
      Serial.println("An error occured! The player bitmask is incorrect.");
      // Serial.print("Slave response: ");
      // Serial.print(slaveResponse);
      // Serial.println("");
    }
  }
}