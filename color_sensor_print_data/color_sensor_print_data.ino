// библиотека для работы с датчиком цвета
#include "TroykaColorSensor.h"
 
// создаём объект для работы с датчиком
TroykaColorSensor colorSensor;
 
// пин подсветки светодиода
#define BACKLIGHT_PIN   6
 
void setup() {
 // инициализируем датчик
 colorSensor.begin();
 // открываем Serial-порт
 Serial.begin(9600);
 
}
void loop() {
 // выставляем уровень подсветки
 analogWrite(BACKLIGHT_PIN,100);
 analogWrite(3,100);
 // считываеv данные в объект RGB
 RGB color = colorSensor.colorRead();
 // выводим показания освещённости каждого цвета
 Serial.print(color.red);
 Serial.print('\t');
 Serial.print(color.green);
 Serial.print('\t');
 Serial.println(color.blue);
 delay(100);
}
