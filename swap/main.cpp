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

String player1="", player2="";

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
Servo motor;

int pl1_pts = 0, pl2_pts = 0;

bool gameStarted = false;

unsigned long timeout = 3000;

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
    }
}

byte sendByte(byte data) {
  digitalWrite(SS, LOW);
  byte slaveResponse = SPI.transfer(data);
  digitalWrite(SS, HIGH);
  Serial.println("Sent byte to slave!");
  return slaveResponse;
}

void initialState() {
  lcd.home();
  lcd.print("Press any button");
  lcd.setCursor(0, 1);
  lcd.print("to begin.");
  byte receivedFromSlave = sendByte(DUMMY);
  if(receivedFromSlave == PL1_BTN_SIG || receivedFromSlave == PL2_BTN_SIG) {
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
  unsigned long startTime = millis();
  while(((slaveResponse & 0b01100000) == 0) && (millis() - startTime < timeout)) {
    slaveResponse = sendByte(DUMMY);
  }
  if(millis() - startTime > timeout) {
    sendByte(TIMEOUT_SIG);

  } else {
    byte color = slaveResponse & 0b01100000;
    slaveResponse = slaveResponse & 0b10011111;
    if(slaveResponse == PL1_BTN_SIG) {
      pl1_pts += millis() - startTime;
    } else if(slaveResponse == PL2_BTN_SIG) {
      pl2_pts += millis() - startTime;
    } else Serial.println("An error occured! The player bitmask is incorrect.");
  }
}