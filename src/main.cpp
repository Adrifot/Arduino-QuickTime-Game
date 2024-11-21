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

String player1="", player2="";

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
Servo motor;

int pl1_pts = 0, pl2_pts = 0;

bool gameStarted = false;
byte sendByte(byte data);
void initialState();

void setup() {
  Serial.begin(9600);

  SPI.begin();
  pinMode(SS, OUTPUT);
  digitalWrite(SS, HIGH);

  lcd.begin(16, 2);

  motor.attach(SERVO_PIN);
  motor.write(0);

  initialState();
}

byte l = 0;

void loop() {
    initialState();
    while(gameStarted) {
        lcd.clear();
        lcd.home();
        lcd.print("Player 1: "); lcd.print(pl1_pts); lcd.print(" pts");
        lcd.setCursor(0, 1);
        lcd.print("Player 2: "); lcd.print(pl2_pts); lcd.print(" pts");
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