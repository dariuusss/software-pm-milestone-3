#include <Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

Adafruit_ST7735 afisor = Adafruit_ST7735(A5,A3,A4);

Servo servos[4];
int servoPins[4] = {9, 6, 5, 3};
int buttonPins[4] = {1, 2, 4, 7}; 
int previousServo = -1;

#define RESET_SCORE_BUTTON 6

int appearances = 0;
int hits = 0;
int misses = 0;

void setup() {
  Serial.begin(9600);
  
  for (int i = 0; i < 4; i++) {
    servos[i].attach(servoPins[i]);
    servos[i].write(0);
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  pinMode(RESET_SCORE_BUTTON, INPUT_PULLUP);

  
  afisor.initR(INITR_BLACKTAB);
  afisor.fillScreen(ST77XX_BLACK);
  afisor.setRotation(1);
  afisor.setTextSize(1);
  afisor.setTextColor(ST77XX_WHITE);

  randomSeed(analogRead(A0));
  afiseaza_scor();
}

void loop() {
 
  if (digitalRead(RESET_SCORE_BUTTON) == LOW) {
    appearances = 0;
    hits = 0;
    misses = 0;
    afiseaza_scor();
    delay(500); 
    return;
  }

  int selected;
  do {
    selected = random(0, 4);
  } while (selected == previousServo);

  previousServo = selected;
  appearances++;

  int potValue = analogRead(A2);
  int totalTime = map(potValue, 0, 1023, 500, 2000);

  servos[selected].write(90);
  unsigned long startTime = millis();
  bool hit = false;

  while (millis() - startTime < totalTime) {
    if (digitalRead(buttonPins[selected]) == LOW) {
      hit = true;
      break;
    }
  }

  servos[selected].write(0);

  if (hit) 
      hits++;
  else 
      misses++;

  afiseaza_scor();
  delay(200);
}

void afiseaza_scor() {
  afisor.fillScreen(ST77XX_BLACK);
  afisor.setCursor(10, 10);
  afisor.print("Whac-A-Mole");
  afisor.setCursor(10, 30);
  afisor.print("Aparitii: ");
  afisor.print(appearances);
  afisor.setCursor(10, 45);
  afisor.print("Lovuturi reusite: ");
  afisor.print(hits);
  afisor.setCursor(10, 60);
  afisor.print("Ratari: ");
  afisor.print(misses);
  afisor.setCursor(10, 75);
  afisor.print("Success: ");
  if (appearances > 0) {
    int success = (hits * 100) / appearances;
    afisor.print(success);
    afisor.print("%");
  } else {
    afisor.print("0%");
  }
}