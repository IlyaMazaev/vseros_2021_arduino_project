#include <Servo.h>
# define mini_servo_1 13
# define mini_servo_2 12
# define mini_servo_3 11
# define servo_1 11
# define servo_0_pin 3

#define BUTTON_PIN  10

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo_0;

void setup() {
  servo1.attach(mini_servo_1);
  servo2.attach(mini_servo_2);
  servo3.attach(mini_servo_3);

servo_0.attach(servo_0_pin);
servo_0.write(88);

}

void loop() {
  int buttonState = digitalRead(BUTTON_PIN);
  while (buttonState){
    buttonState = digitalRead(BUTTON_PIN);
    delay (100);
  }while (not buttonState){
    buttonState = digitalRead(BUTTON_PIN);
  }
  servo_0.write(180);
  delay (900);
  servo_0.write(88);
  delay (900);
  servo_0.write(180);
  delay (810);
  servo_0.write(88);
  delay (900);
  servo_0.write(180);
  delay (810);
  servo_0.write(88);
  delay (900);

  servo_0.write(180);
  delay (900);
  servo_0.write(88);
  delay (500);
  
  servo_0.write(0);
  delay (3300);
  servo_0.write(88);


}
