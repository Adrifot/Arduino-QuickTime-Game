// SLAVE
#include <Arduino.h>

#define BTN_PL1 2
#define BTN_PL2 3
#define R1 4
#define G1 5
#define B1 6
#define R2 7
#define G2 8
#define B2 9

void player1_ISR() {
    int readVal = analogRead(A0);
    Serial.print("Player 1: ");
    Serial.print(readVal);
    Serial.println("");
}

void player2_ISR() {
    int readVal = analogRead(A1);
    Serial.print("Player 2: ");
    Serial.print(readVal);
    Serial.println("");
}



void setup() {
    Serial.begin(9600);
    attachInterrupt(digitalPinToInterrupt(BTN_PL1), player1_ISR, RISING);
    attachInterrupt(digitalPinToInterrupt(BTN_PL2), player2_ISR, RISING);
    for(int i=R1; i<=B2; i++) {
        pinMode(i, OUTPUT);
    }
}

void loop() {
    
}