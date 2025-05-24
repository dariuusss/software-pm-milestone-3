#include <Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

Adafruit_ST7735 afisor = Adafruit_ST7735(A5,A3,A4);

Servo servos[4];
int servoPins[4] = {9, 6, 5, 3};
int buttonPins[4] = {1, 2, 4, 7}; 
int previous_servo = -1;

#define RESET_BUTTON 8
#define START_GAME 100
#define CHOOSE_MOLE 101
#define SURFACE_MOLE 102
#define REFRESH_SCORE 103
#define RESET_WAIT 104
#define GAME_OVER 105

int appearances = 0;
int hits = 0;
int misses = 0;

int state = START_GAME;

unsigned long state_start_time = 0;
unsigned long game_start_time = 0;
int selected = -1;
bool hit = false;
unsigned long reaction_time = 1000;

volatile bool game_time_expired = false;
volatile unsigned int ticks = 0;

ISR(TIMER2_COMPA_vect) {
  ticks++;
  if (ticks >= 3750) { 
    game_time_expired = true;
  }
}

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 4; i++) {
    servos[i].attach(servoPins[i]);
    servos[i].write(0);
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  pinMode(RESET_BUTTON, INPUT_PULLUP);

  afisor.initR(INITR_BLACKTAB);
  afisor.fillScreen(ST77XX_BLACK);
  afisor.setRotation(1);
  afisor.setTextSize(1);
  afisor.setTextColor(ST77XX_WHITE);

  randomSeed(analogRead(A0));
  afiseaza_scor();

  cli();
  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2 = 0;
  OCR2A = 249;
  TCCR2A |= (1 << WGM21);
  TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20); 
  TIMSK2 |= (1 << OCIE2A);
  sei();
}

void loop() {
  int pot_value = analogRead(A2);
  reaction_time = map(pot_value, 0, 1023, 500, 2000);

  if (game_time_expired && state != GAME_OVER) {
    state = GAME_OVER;
    afisor.fillScreen(ST77XX_BLACK);
    afisor.setCursor(20, 40);
    afisor.setTextColor(ST77XX_RED);
    afisor.setTextSize(2);
    afisor.print("TIME'S UP");
    afisor.setTextSize(1);
    delay(3000);
    afiseaza_scor();
    return;
  }

  if (state == START_GAME) {
    game_start_time = millis();
    state = CHOOSE_MOLE;
  }
  else if (state == CHOOSE_MOLE) {
    if (digitalRead(RESET_BUTTON) == LOW) {
      state = RESET_WAIT;
      state_start_time = millis();
      return;
    }
    do {
      selected = random(0, 4);
    } while (selected == previous_servo);
    previous_servo = selected;

    servos[selected].write(90);
    state_start_time = millis();
    state = SURFACE_MOLE;
    hit = false;
    appearances++;
  }
  else if (state == SURFACE_MOLE) {
    if (digitalRead(buttonPins[selected]) == LOW) {
      if (millis() - state_start_time < reaction_time)
          hit = true;
      servos[selected].write(0);
      state_start_time = millis();
      state = REFRESH_SCORE;
    } else if (millis() - state_start_time >= reaction_time) {
      servos[selected].write(0);
      state_start_time = millis();
      state = REFRESH_SCORE;
    }
  }
  else if (state == REFRESH_SCORE) {
    if (millis() - state_start_time >= 200) {
      if (hit) 
        hits++;
    else 
        misses++;
      afiseaza_scor();
      state = CHOOSE_MOLE;
    }
  }
  else if (state == RESET_WAIT) {
    if (millis() - state_start_time >= 500) {
      if (digitalRead(RESET_BUTTON) != LOW) {

         appearances = hits = misses = 0;
         afiseaza_scor();
         state = START_GAME;
         ticks = 0;
         game_time_expired = false;
      }
    }
  } else if (state == GAME_OVER) {
    ;
  }
}

void afiseaza_scor() {
  afisor.fillScreen(ST77XX_BLACK);
  afisor.setCursor(10, 10);
  afisor.print("Whac-A-Mole");
  afisor.setCursor(10, 30);
  afisor.print("Aparitii: "); afisor.print(appearances);
  afisor.setCursor(10, 45);
  afisor.print("Lovituri reusite: "); afisor.print(hits);
  afisor.setCursor(10, 60);
  afisor.print("Ratari: "); afisor.print(misses);
  afisor.setCursor(10, 75);
  afisor.print("Procent lovituri reusite: ");
  if (appearances > 0) {
    int success = (hits * 100) / appearances;
    afisor.print(success); afisor.print("%");
  } else {
    afisor.print("0%");
  }
}