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

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo_0;

TroykaColorSensor colorSensor;

#define LIGHT_SENSOR_0_PIN   5 
#define LIGHT_SENSOR_1_PIN   6
#define LIGHT_SENSOR_2_PIN   7

void setup() {
  Serial.begin(9600);
  servo1.attach(mini_servo_1);
  servo2.attach(mini_servo_2);
  servo3.attach(mini_servo_3);

  analogWrite(LIGHT_SENSOR_0_PIN,100); // подсветка датчиков вкл
analogWrite(LIGHT_SENSOR_1_PIN,100);
analogWrite(LIGHT_SENSOR_2_PIN,100);
while (1) {
  splitter.setBusChannel(SENSOR_0_HUB_PIN);
  RGB color_0 = colorSensor.colorRead();

  splitter.setBusChannel(SENSOR_1_HUB_PIN);
  RGB color_1 = colorSensor.colorRead();

  splitter.setBusChannel(SENSOR_2_HUB_PIN);
  RGB color_2 = colorSensor.colorRead();
  Serial.print(color_0.red);
 Serial.print('\t');
 Serial.print(color_0.green);
 Serial.print('\t');
 Serial.println(color_0.blue);
 Serial.print('\t');
 Serial.print('\t');
 Serial.print(color_1.red);
 Serial.print('\t');
 Serial.print(color_1.green);
 Serial.print('\t');
 Serial.println(color_1.blue);
 Serial.print('\t');
 Serial.print('\t');
 Serial.print(color_2.red);
 Serial.print('\t');
 Serial.print(color_2.green);
 Serial.print('\t');
 Serial.println(color_2.blue);
  if (color_0.red != 0 and color_0.green != 0 and color_0.blue != 0 and color_1.red != 0 and color_1.green != 0 and color_1.blue != 0 and color_2.red != 0 and color_2.green != 0 and color_2.blue != 0) break;
}
analogWrite(LIGHT_SENSOR_0_PIN,0); // подсветка датчиков вкл
analogWrite(LIGHT_SENSOR_1_PIN,0);
analogWrite(LIGHT_SENSOR_2_PIN,0);
     

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
