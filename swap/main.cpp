// SLAVE
#include <Arduino.h>
#include <SPI.h>

#define BTN_PL1 2
#define BTN_PL2 3
#define R1 4
#define G1 5
#define B1 6
#define R2 7
#define G2 8
#define B2 9

volatile byte receivedByte;

void player1_ISR();
void player2_ISR();
void setRGB(int player, int color);

ISR(SPI_STC_vect) {
    Serial.println("SLAVE RECEIVING STUFF!");
    receivedByte = SPDR;
    setRGB(1, receivedByte);
}

void setup() {
    Serial.begin(9600);
    SPI.begin();
    pinMode(MISO, OUTPUT);
    SPI.attachInterrupt();
    SPCR |= _BV(SPE);
    // attachInterrupt(digitalPinToInterrupt(BTN_PL1), player1_ISR, RISING);
    // attachInterrupt(digitalPinToInterrupt(BTN_PL2), player2_ISR, RISING);
    for(int i=R1; i<=B2; i++) {
        pinMode(i, OUTPUT);
    }
}

void loop() {
    Serial.println("SLAVE ALIVE");
    // setRGB(1, value) where value was sent by the master
}

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

void setRGB(int player, int color) {
    int r, g, b;
    if(player == 1) {
        r = 4; g = 5; b = 6;
    } else if(player == 2) {
        r = 7; g = 8; b = 9;
    } else {
        Serial.println("Player select error");
        return;
    }
    Serial.print("Player ");
    Serial.print(player);
    Serial.println("");
    Serial.print("Color ");
    Serial.print(color);
    Serial.println("");
    Serial.println("");
    switch(color) {
        case 0:
            digitalWrite(r, LOW);
            digitalWrite(g, LOW);
            digitalWrite(b, LOW);
            break;
        case 1:
            digitalWrite(r, HIGH);
            digitalWrite(g, LOW);
            digitalWrite(b, LOW);
            break;
        case 2:
            digitalWrite(r, LOW);
            digitalWrite(g, HIGH);
            digitalWrite(b, LOW);
            break;
        case 3:
            digitalWrite(r, LOW);
            digitalWrite(g, LOW);
            digitalWrite(b, HIGH);
            break;
        case 4:
            digitalWrite(r, HIGH);
            digitalWrite(g, HIGH);
            digitalWrite(b, HIGH);
            break;
        default:
            Serial.println("Color select error.");
            // digitalWrite(r, HIGH);
            // digitalWrite(g, LOW);
            // digitalWrite(b, HIGH);
            return;
    }
}