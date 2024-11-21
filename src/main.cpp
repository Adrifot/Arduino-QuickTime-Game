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

String player1="", player2="";

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
Servo motor;

byte color1 = 1;
byte color2 = 2;

byte sendByte(byte data);

void setup() {
  Serial.begin(9600);

  SPI.begin();
  pinMode(SS, OUTPUT);
  digitalWrite(SS, HIGH);
  // lcd.begin(16, 2);
  // motor.attach(SERVO_PIN);
  
}

byte l = 0;

void loop() {
  // every second, send the value of 0, 1, 2, 3 and then 4to the slave
  sendByte(l);
  l = (l+1)%5;
  delay(1000);
}

byte sendByte(byte data) {
  digitalWrite(SS, LOW);
  byte slaveResponse = SPI.transfer(data);
  digitalWrite(SS, HIGH);
  Serial.println("Sent byte to slave!");
  // return slaveResponse;
}