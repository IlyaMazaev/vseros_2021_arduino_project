#include <Servo.h>
# define mini_servo_1 13
# define mini_servo_2 12
# define mini_servo_3 11
# define servo_1 11

Servo servo1;
Servo servo2;
Servo servo3;

void setup() {
  servo1.attach(mini_servo_1);
  servo2.attach(mini_servo_2);
  servo3.attach(mini_servo_3);


}

void loop() {
  servo1.write(0);
  delay(1000);
  servo2.write(0);
  delay(1000);
  servo3.write(0);
  delay(1000);
  servo1.write(180);
  delay(1000);
  servo2.write(180);
  delay(1000);
  servo3.write(180);
  delay(1000);


}
