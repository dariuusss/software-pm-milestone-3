#include <Servo.h>

Servo servos[4];
int pins[4] = {9, 6, 5, 3};
int previousServo = -1;

void setup() {
  for (int i = 0; i < 4; i++) {
    servos[i].attach(pins[i]);
    servos[i].write(0);
  }

  randomSeed(analogRead(A0)); 
}

void loop() {
  int selected;

  
  do {
    selected = random(0, 4);
  } while (selected == previousServo);

  previousServo = selected;

  
  int potValue = analogRead(A2);
  int delayTime = map(potValue, 0, 1023, 500, 2000);

  servos[selected].write(90);
  delay(delayTime / 2);   
  servos[selected].write(0);
  delay(delayTime / 2);   
}
