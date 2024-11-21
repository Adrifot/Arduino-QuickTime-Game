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

#define DUMMY 255
#define PL1_BTN_SIG 10
#define PL2_BTN_SIG 20

#define R_MIN 910
#define R_MAX 970
#define G_MIN 830
#define G_MAX 890
#define B_MIN 680
#define B_MAX 740

volatile byte receivedByte, responseByte;

void player1_ISR();
void player2_ISR();
void setRGB(int player, int color);
void ledCycle(byte c1, byte c2);
byte mapColor(int analogData);

ISR(SPI_STC_vect) {
    Serial.print("Slave received value: ");
    receivedByte = SPDR;
    Serial.print(receivedByte);
    Serial.println("");
    if(receivedByte >= 0 && receivedByte <= 15) {
        byte col1 = receivedByte & 3;
        byte col2 = (receivedByte >> 2) & 3;
        ledCycle(col1, col2);
    }
}

void setup() {
    Serial.begin(9600);
    pinMode(SCK, INPUT);
    pinMode(MOSI, INPUT);
    pinMode(MISO, OUTPUT);
    pinMode(SS, INPUT);
    SPI.attachInterrupt();
    SPCR |= _BV(SPE);
    attachInterrupt(digitalPinToInterrupt(BTN_PL1), player1_ISR, RISING);
    attachInterrupt(digitalPinToInterrupt(BTN_PL2), player2_ISR, RISING);
    for(int i=R1; i<=B2; i++) {
        pinMode(i, OUTPUT);
    }
}

void loop() {
    // Serial.println("SLAVE ALIVE");
    // setRGB(1, value) where value was sent by the master
}

void player1_ISR() {
    int readVal = analogRead(A0);
    Serial.print("Player 1: ");
    Serial.print(readVal);
    Serial.println("");
    responseByte = PL1_BTN_SIG;
    byte colorCode = mapColor(readVal);
    responseByte |= (colorCode << 5);
    SPDR = responseByte;
}

void player2_ISR() {
    int readVal = analogRead(A1);
    Serial.print("Player 2: ");
    Serial.print(readVal);
    Serial.println("");
    responseByte = PL2_BTN_SIG;
    byte colorCode = mapColor(readVal);
    responseByte |= (colorCode << 5);
    SPDR = responseByte;
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

void ledCycle(byte c1, byte c2) {
    setRGB(1, c1);
    setRGB(1, c2);
}

byte mapColor(int analogData) {
    if(analogData >= R_MIN && analogData <= R_MAX) return 1;
    else if(analogData >= G_MIN && analogData <= G_MAX) return 2;
    else if(analogData >= B_MIN && analogData <= B_MAX) return 3;
    else return 0;
}