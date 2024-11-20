// MASTER
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Servo.h>

const int rs = 4, en = 5, d4 = 6, d5 = 7, d6 = 8, d7 = 9;
const int motorPin = 3;
String player1="", player2="";

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
Servo motor;

void getNames() {
  lcd.clear();
  
  lcd.print("Player 1 Name:");
  Serial.println("Enter Player 1 Name:");
  while (Serial.available() == 0);  
  player1 = Serial.readStringUntil('\n');  
  lcd.setCursor(0, 1);
  lcd.print(player1);  
  delay(2000);         
  
  lcd.clear();
  
  lcd.print("Player 2 Name:");
  Serial.println("Enter Player 2 Name:");
  while (Serial.available() == 0);  
  player2 = Serial.readStringUntil('\n');  
  lcd.setCursor(0, 1);
  lcd.print(player2);  
  delay(2000);         
  
  lcd.clear();
  
  // Final display
  lcd.setCursor(0, 0);
  lcd.print(player1 + ":");
  lcd.setCursor(0, 1);
  lcd.print(player2 + ":");
}

void setup() {
  lcd.begin(16, 2);
  motor.attach(motorPin);
  Serial.begin(9600);
  
}

int i = 0;

void loop() {
  while(i == 0) {
    getNames();
    i++;
  }
}