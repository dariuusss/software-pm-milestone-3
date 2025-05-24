#include <Servo.h>

Servo servos[4];
int servoPins[4] = {9, 6, 5, 3};
int buttonPins[4] = {1, 2, 4, 7}; 
int previousServo = -1;

void setup() {
 
  for (int i = 0; i < 4; i++) {
    servos[i].attach(servoPins[i]);
    servos[i].write(0);
    pinMode(buttonPins[i], INPUT_PULLUP); 
  }

  Serial.begin(9600);
  randomSeed(analogRead(A0)); 
}

void loop() {
  int selected;

  do {
    selected = random(0, 4);
  } while (selected == previousServo);

  previousServo = selected;

  
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

  if (hit) {
    Serial.print("Ai nimerit");
  } else {
    Serial.print("Ai ratat");
  }
   
  delay(200); 
}