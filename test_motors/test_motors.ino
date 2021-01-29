#include <Servo.h>
#include <TroykaI2CHub.h>

// библиотека для работы с датчиком цвета
#include <TroykaColorSensor.h>

// создаём объект для работы с хабом
// адрес по умолчанию 0x70
TroykaI2CHub splitter;

# define mini_servo_1 13
# define mini_servo_2 12
# define mini_servo_3 11
# define servo_1 11
# define servo_0_pin 3

#define BUTTON_PIN  10
#define SENSOR_0_HUB_PIN   0 
#define SENSOR_1_HUB_PIN   3
#define SENSOR_2_HUB_PIN   5

# define mini_servo_0_pin 13
# define mini_servo_1_pin 12
# define mini_servo_2_pin 11

# define servo_0_pin 3
# define servo_0_stop_speed 89

Servo servo_0_mini;
Servo servo_1_mini;
Servo servo_2_mini;
Servo servo_0;

TroykaColorSensor colorSensor;

#define LIGHT_SENSOR_0_PIN   5 
#define LIGHT_SENSOR_1_PIN   6
#define LIGHT_SENSOR_2_PIN   7

void setup() {
  Serial.begin(9600);
  servo_0_mini.attach(mini_servo_0_pin);
  servo_1_mini.attach(mini_servo_1_pin);
  servo_2_mini.attach(mini_servo_2_pin);

  servo_0.attach(servo_0_pin);
  servo_0.write(servo_0_stop_speed);
  
  servo_0_mini.write(180);
  servo_1_mini.write(180);
  servo_2_mini.write(180);
     

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
  delay (3350);
  servo_0.write(servo_0_stop_speed);

  makeStep(2, 1);


}

void makeStep(int step_i, int step_j){
  if (step_i == 0){
    servo_0.write(0);
  delay (1600);
  servo_0.write(servo_0_stop_speed);
  delay(100);
    if (step_j == 0){
      servo_0_mini.write(0);
      delay(500);
      servo_0_mini.write(180);
    }
    else if (step_j == 1){
      servo_1_mini.write(0);
      delay(500);
      servo_1_mini.write(180);
    }
    else if (step_j == 2){
      servo_2_mini.write(0);
      delay(500);
      servo_2_mini.write(180);
    }
   servo_0.write(0);
   delay (1850);
   servo_0.write(servo_0_stop_speed);
  }
  else if (step_i == 1){
    servo_0.write(0);
  delay (800);
  servo_0.write(servo_0_stop_speed);
  delay(100);
    if (step_j == 0){
      servo_0_mini.write(0);
      delay(500);
      servo_0_mini.write(180);
    }
    else if (step_j == 1){
      servo_1_mini.write(0);
      delay(500);
      servo_1_mini.write(180);
    }
    else if (step_j == 2){
      servo_2_mini.write(0);
      delay(500);
      servo_2_mini.write(180);
    }
   servo_0.write(0);
   delay (2640);
   servo_0.write(servo_0_stop_speed);
  }
  else if (step_i == 2){
  servo_0.write(servo_0_stop_speed);
    if (step_j == 0){
      servo_0_mini.write(0);
      delay(500);
      servo_0_mini.write(180);
    }
    else if (step_j == 1){
      servo_1_mini.write(0);
      delay(500);
      servo_1_mini.write(180);
    }
    else if (step_j == 2){
      servo_2_mini.write(0);
      delay(500);
      servo_2_mini.write(180);
    }
   servo_0.write(0);
   delay (3240);
   servo_0.write(servo_0_stop_speed);
  }
}
