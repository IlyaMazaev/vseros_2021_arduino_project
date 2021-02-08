#include <Servo.h>

Servo servo_0_mini;
Servo servo_1_mini;
Servo servo_2_mini;

# define mini_servo_0_pin 13
# define mini_servo_1_pin 12
# define mini_servo_2_pin 11
  


void setup() {
  servo_0_mini.attach(mini_servo_0_pin);
  servo_1_mini.attach(mini_servo_1_pin);
  servo_2_mini.attach(mini_servo_2_pin);
  
  servo_0_mini.write(180);
  servo_1_mini.write(180);
  servo_2_mini.write(180);
  delay(5000);

}

void loop() {
  servo_0_mini.write(0);
  servo_1_mini.write(0);
  servo_2_mini.write(0);
  delay(500);
  servo_0_mini.write(180);
  servo_1_mini.write(180);
  servo_2_mini.write(180);
  delay(500);

}
