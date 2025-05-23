#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

void setup() {
  servo1.attach(9);
  servo2.attach(6);
  servo3.attach(5);
  servo4.attach(3);
}

void loop() {
  
  servo1.write(90);
  delay(1000);
  servo1.write(0);
  delay(1000);

  
  servo2.write(90);
  delay(1000);
  servo2.write(0);
  delay(1000);

  
  servo3.write(90);
  delay(1000);
  servo3.write(0);
  delay(1000);

 
  servo4.write(90);
  delay(1000);
  servo4.write(0);
  delay(1000);
}