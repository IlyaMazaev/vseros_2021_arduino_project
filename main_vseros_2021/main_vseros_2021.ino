#include <TroykaI2CHub.h>

// библиотека для работы с датчиком цвета
#include <TroykaColorSensor.h>
#include <Servo.h>
# define mini_servo_0_pin 11
# define mini_servo_1_pin 12
# define mini_servo_2_pin 13

# define servo_0_pin 3
# define servo_0_stop_speed 89

Servo servo_0_mini;
Servo servo_1_mini;
Servo servo_2_mini;

Servo servo_0;

#define BUTTON_PIN  10

#define SOUND_PIN 7

#define color_value 240  // пороговое значение определения цветов
// если например color.blue > color_value , то я считаю, что показание датчика = синий цвет

// пин подсветки светодиода
#define LIGHT_SENSOR_0_PIN   5 
#define LIGHT_SENSOR_1_PIN   6
#define LIGHT_SENSOR_2_PIN   8

#define SENSOR_0_HUB_PIN   0
#define SENSOR_1_HUB_PIN   2
#define SENSOR_2_HUB_PIN   4

// создаём объект для работы с датчиком
TroykaColorSensor colorSensor;

// создаём объект для работы с хабом
// адрес по умолчанию 0x70
TroykaI2CHub splitter;

int board[3][3] = { // создание доски
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}
    };

int step_pos[2] = {4, 4}; // координаты хода

 
void setup() {
  servo_0_mini.attach(mini_servo_0_pin);
  servo_1_mini.attach(mini_servo_1_pin);
  servo_2_mini.attach(mini_servo_2_pin);

  servo_0_mini.write(0);
  servo_1_mini.write(0);
  servo_2_mini.write(0);
  
  servo_0.attach(servo_0_pin);
  servo_0.write(servo_0_stop_speed);  // остановка привода поля
  
  pinMode(BUTTON_PIN, INPUT);
  pinMode(LIGHT_SENSOR_0_PIN, OUTPUT);
  pinMode(LIGHT_SENSOR_1_PIN, OUTPUT); 
  pinMode(LIGHT_SENSOR_2_PIN, OUTPUT);  
 // инициализируем датчик
 colorSensor.begin();
 // открываем Serial-порт
 Serial.begin(9600);

digitalWrite(LIGHT_SENSOR_0_PIN,HIGH); // подсветка датчиков вкл
digitalWrite(LIGHT_SENSOR_1_PIN,HIGH);
digitalWrite(LIGHT_SENSOR_2_PIN,HIGH);
Serial.println(123);



// цикл для определения ошибок в работе датчика
// если датчик выдаёт 0 0 0, то из цикла программа не выйдет, пока он не запустится
while (1) {
  TroykaColorSensor colorSensor;
  
  splitter.setBusChannel(SENSOR_0_HUB_PIN);
  RGB color_0 = colorSensor.colorRead();

  splitter.setBusChannel(SENSOR_1_HUB_PIN);
  RGB color_1 = colorSensor.colorRead();

  splitter.setBusChannel(SENSOR_2_HUB_PIN);
  RGB color_2 = colorSensor.colorRead();
  
  if (color_0.red == 0 and color_0.green == 0 and color_0.blue == 0){
    Serial.println(0);
    digitalWrite(LIGHT_SENSOR_0_PIN,LOW);
  }
  if (color_1.red == 0 and color_1.green == 0 and color_1.blue == 0){
    Serial.println(1);
    digitalWrite(LIGHT_SENSOR_1_PIN,LOW);
  }
  if (color_2.red == 0 and color_2.green == 0 and color_2.blue == 0){
    Serial.println(2);
    digitalWrite(LIGHT_SENSOR_2_PIN,LOW);
  }

  if (color_0.red != 0 and color_0.green != 0 and color_0.blue != 0 and color_1.red != 0 and color_1.green != 0 and color_1.blue != 0 and color_2.red != 0 and color_2.green != 0 and color_2.blue != 0){
    Serial.print(color_0.red + color_0.green + color_0.blue);
Serial.print("\t");
Serial.print(color_1.red + color_1.green + color_1.blue);
Serial.print("\t");
Serial.println(color_2.red + color_2.green + color_2.blue);
    break;
  }
  }


digitalWrite(LIGHT_SENSOR_0_PIN,LOW); // подсветка датчиков выкл
digitalWrite(LIGHT_SENSOR_1_PIN,LOW);
digitalWrite(LIGHT_SENSOR_2_PIN,LOW);




tone(SOUND_PIN, 523, 150);
delay(250);
tone(SOUND_PIN, 659, 150);
delay(250);
tone(SOUND_PIN, 784, 150);
delay(250);
tone(SOUND_PIN, 523, 200);
}



void loop() {
  servo_0_mini.write(0);
  servo_1_mini.write(0);
  servo_2_mini.write(0);
  
  int buttonState = digitalRead(BUTTON_PIN);
  // ожидание нажатия кнопки 
  while (buttonState){
    buttonState = digitalRead(BUTTON_PIN);
  }
  delay (50);
  // ожидание отпускания кнопки 
  while (not buttonState){
    buttonState = digitalRead(BUTTON_PIN);
  }
  tone(SOUND_PIN, 494, 100);
   
  scanBoard(); // сканирование поля
  if (not checkWin()){  // если на поле нет победы
    makeRandomStep();  // сначала нахожу координаты обычного хода
    closeLineOfTwo();  // если нужно, то тут координаты хода меняются на координаты закрытия линии из 2х
    Serial.print(step_pos[0]); // вывод координат хода в консоль
    Serial.print(" ");
    Serial.println(step_pos[1]);
    makeStep(step_pos[0], step_pos[1]);  // кладу шарик в нужную клетку
  }
  else{
   servo_0.write(180);  // если есть победа, то поле возвращается в крайнее ближнее положение
   delay (3240);
   servo_0.write(servo_0_stop_speed);
   servo_0.write(0);
   tone(SOUND_PIN,1318,150);
   delay (150);
   servo_0.write(servo_0_stop_speed);
   servo_0.write(180);
   tone(SOUND_PIN,1318,300);
   delay(150);
   servo_0.write(servo_0_stop_speed);
   delay(150);
   servo_0.write(0);
   tone(SOUND_PIN,1318,150);
  delay(300);
   servo_0.write(servo_0_stop_speed);
   tone(SOUND_PIN,1046,150);
    delay(150);
   servo_0.write(servo_0_stop_speed);

   tone(SOUND_PIN,1318,300);
  servo_0.write(180);
  delay(300);
  servo_0.write(servo_0_stop_speed);
  tone(SOUND_PIN,1568,600);
  delay(600);
  tone(SOUND_PIN,784,600);
  servo_0.write(180);
  delay(200);
  servo_0.write(servo_0_stop_speed);






  }
}



void scanBoard() {  
    // сканирование поля и его перезапись(поле должно находиться в крайнем нижнем положении)
     digitalWrite(LIGHT_SENSOR_0_PIN,HIGH); // подсветка датчиков вкл
     digitalWrite(LIGHT_SENSOR_1_PIN,HIGH);
     digitalWrite(LIGHT_SENSOR_2_PIN,HIGH);

     servo_0.write(0); // двигаю 0 строчку к датчикам
     delay (900);
     servo_0.write(servo_0_stop_speed);
     
     splitter.setBusChannel(SENSOR_0_HUB_PIN); // переключаю канал шины i2c на первый датчик
     int red_average = 0;
     int green_average = 0; // переменные для расчёта данных с датчиков
     int blue_average = 0;
     for (int i = 0; i < 10; i++) {  // для считывания результатов 10 измерений для повышения надёжности
        RGB color = colorSensor.colorRead(); // считываю новые данные в объект RGB
        
        red_average  += color.red;
        green_average += color.green;
        blue_average += color.blue;
      }
     red_average = red_average / 10;  // делю на 10 для нахождения среднего значения измерений
     green_average = green_average / 10;
     blue_average = blue_average / 10;
     // если преобладает какой-либо цвет, то записываю в массив и вывожу в консоль
     if (green_average > color_value){ 
      Serial.print("1");
      board[0][0] = 1;
     }
     else if (red_average > color_value){
      Serial.print("0");
      board[0][0] = 0;
     }
     else if (blue_average > color_value){
      Serial.print("2");
      board[0][0] = 2;
     }
     Serial.print('\t');
     
     // аналогично для другого датчика
     splitter.setBusChannel(SENSOR_1_HUB_PIN); // переключаю канал шины i2c на второй датчик
     red_average = 0;
     green_average = 0; // переменные для расчёта данных с датчиков
     blue_average = 0;
     for (int i = 0; i < 10; i++) {  // для считывания результатов 10 измерений для повышения надёжности
        RGB color = colorSensor.colorRead(); // считываю новые данные в объект RGB
        red_average  += color.red;
        green_average += color.green;
        blue_average += color.blue;
      }
     red_average = red_average / 10;  // делю на 10 для нахождения среднего значения измерений
     green_average = green_average / 10;
     blue_average = blue_average / 10;
     // если преобладает какой-либо цвет, то записываю в массив и вывожу в консоль
     if (green_average > color_value){ 
      Serial.print("1");
      board[0][1] = 1;
     }
     else if (red_average > color_value){
      Serial.print("0");
      board[0][1] = 0;
     }
     else if (blue_average > color_value){
      Serial.print("2");
      board[0][1] = 2;
     }
     Serial.print('\t');

     // аналогично для другого датчика
     splitter.setBusChannel(SENSOR_2_HUB_PIN); // переключаю канал шины i2c на третий датчик
     red_average = 0;
     green_average = 0; // переменные для расчёта данных с датчиков
     blue_average = 0;
     for (int i = 0; i < 10; i++) {  // для считывания результатов 10 измерений для повышения надёжности
        RGB color = colorSensor.colorRead(); // считываю новые данные в объект RGB
        red_average  += color.red;
        green_average += color.green;
        blue_average += color.blue;
      }
     red_average = red_average / 10;  // делю на 10 для нахождения среднего значения измерений
     green_average = green_average / 10;
     blue_average = blue_average / 10;
     // если преобладает какой-либо цвет, то записываю в массив и вывожу в консоль
     if (green_average > color_value){ 
      Serial.print("1");
      board[0][2] = 1;
     }
     else if (red_average > color_value){
      Serial.print("0");
      board[0][2] = 0;
     }
     else if (blue_average > color_value){
      Serial.print("2");
      board[0][2] = 2;
     }
     Serial.print(red_average);
     Serial.print(' ');
     Serial.print(green_average);
     Serial.print(' ');
     Serial.print(blue_average);
     Serial.print(' ');
     
     Serial.println('\t');

     servo_0.write(0);  // двигаю следующую строчку
     delay (810);
     servo_0.write(servo_0_stop_speed);

     splitter.setBusChannel(SENSOR_0_HUB_PIN); // переключаю канал шины i2c на первый датчик
     red_average = 0;
     green_average = 0; // переменные для расчёта данных с датчиков
     blue_average = 0;
     for (int i = 0; i < 10; i++) {  // для считывания результатов 10 измерений для повышения надёжности
        RGB color = colorSensor.colorRead(); // считываю новые данные в объект RGB
        red_average  += color.red;
        green_average += color.green;
        blue_average += color.blue;
      }
     red_average = red_average / 10;  // делю на 10 для нахождения среднего значения измерений
     green_average = green_average / 10;
     blue_average = blue_average / 10;
     // если преобладает какой-либо цвет, то записываю в массив и вывожу в консоль
     if (green_average > color_value){ 
      Serial.print("1");
      board[1][0] = 1;
     }
     else if (red_average > color_value){
      Serial.print("0");
      board[1][0] = 0;
     }
     else if (blue_average > color_value){
      Serial.print("2");
      board[1][0] = 2;
     }
     Serial.print('\t');
     
     // аналогично для другого датчика
     splitter.setBusChannel(SENSOR_1_HUB_PIN); // переключаю канал шины i2c на второй датчик
     red_average = 0;
     green_average = 0; // переменные для расчёта данных с датчиков
     blue_average = 0;
     for (int i = 0; i < 10; i++) {  // для считывания результатов 10 измерений для повышения надёжности
        RGB color = colorSensor.colorRead(); // считываю новые данные в объект RGB
        red_average  += color.red;
        green_average += color.green;
        blue_average += color.blue;
      }
     red_average = red_average / 10;  // делю на 10 для нахождения среднего значения измерений
     green_average = green_average / 10;
     blue_average = blue_average / 10;
     // если преобладает какой-либо цвет, то записываю в массив и вывожу в консоль
     if (green_average > color_value){ 
      Serial.print("1");
      board[1][1] = 1;
     }
     else if (red_average > color_value){
      Serial.print("0");
      board[1][1] = 0;
     }
     else if (blue_average > color_value){
      Serial.print("2");
      board[1][1] = 2;
     }
     Serial.print('\t');

     // аналогично для другого датчика
     splitter.setBusChannel(SENSOR_2_HUB_PIN); // переключаю канал шины i2c на третий датчик
     red_average = 0;
     green_average = 0; // переменные для расчёта данных с датчиков
     blue_average = 0;
     for (int i = 0; i < 10; i++) {  // для считывания результатов 10 измерений для повышения надёжности
        RGB color = colorSensor.colorRead(); // считываю новые данные в объект RGB
        red_average  += color.red;
        green_average += color.green;
        blue_average += color.blue;
      }
     red_average = red_average / 10;  // делю на 10 для нахождения среднего значения измерений
     green_average = green_average / 10;
     blue_average = blue_average / 10;
     // если преобладает какой-либо цвет, то записываю в массив и вывожу в консоль
     if (green_average > color_value){ 
      Serial.print("1");
      board[1][2] = 1;
     }
     else if (red_average > color_value){
      Serial.print("0");
      board[1][2] = 0;
     }
     else if (blue_average > color_value){
      Serial.print("2");
      board[1][2] = 2;
     }
     Serial.print(red_average);
     Serial.print(' ');
     Serial.print(green_average);
     Serial.print(' ');
     Serial.print(blue_average);
     Serial.print(' ');
     
     Serial.println('\t');

     servo_0.write(0);  // двигаю следующую строчку
     delay (810);
     servo_0.write(servo_0_stop_speed);

     splitter.setBusChannel(SENSOR_0_HUB_PIN); // переключаю канал шины i2c на первый датчик
     red_average = 0;
     green_average = 0; // переменные для расчёта данных с датчиков
     blue_average = 0;
     for (int i = 0; i < 10; i++) {  // для считывания результатов 10 измерений для повышения надёжности
        RGB color = colorSensor.colorRead(); // считываю новые данные в объект RGB
        red_average  += color.red;
        green_average += color.green;
        blue_average += color.blue;
      }
     red_average = red_average / 10;  // делю на 10 для нахождения среднего значения измерений
     green_average = green_average / 10;
     blue_average = blue_average / 10;
     // если преобладает какой-либо цвет, то записываю в массив и вывожу в консоль
     if (green_average > color_value){ 
      Serial.print("1");
      board[2][0] = 1;
     }
     else if (red_average > color_value){
      Serial.print("0");
      board[2][0] = 0;
     }
     else if (blue_average > color_value){
      Serial.print("2");
      board[2][0] = 2;
     }
     Serial.print('\t');
     
     // аналогично для другого датчика
     splitter.setBusChannel(SENSOR_1_HUB_PIN); // переключаю канал шины i2c на второй датчик
     red_average = 0;
     green_average = 0; // переменные для расчёта данных с датчиков
     blue_average = 0;
     for (int i = 0; i < 10; i++) {  // для считывания результатов 10 измерений для повышения надёжности
        RGB color = colorSensor.colorRead(); // считываю новые данные в объект RGB
        red_average  += color.red;
        green_average += color.green;
        blue_average += color.blue;
      }
     red_average = red_average / 10;  // делю на 10 для нахождения среднего значения измерений
     green_average = green_average / 10;
     blue_average = blue_average / 10;
     // если преобладает какой-либо цвет, то записываю в массив и вывожу в консоль
     if (green_average > color_value){ 
      Serial.print("1");
      board[2][1] = 1;
     }
     else if (red_average > color_value){
      Serial.print("0");
      board[2][1] = 0;
     }
     else if (blue_average > color_value){
      Serial.print("2");
      board[2][1] = 2;
     }
     Serial.print('\t');

     // аналогично для другого датчика
     splitter.setBusChannel(SENSOR_2_HUB_PIN); // переключаю канал шины i2c на третий датчик
     red_average = 0;
     green_average = 0; // переменные для расчёта данных с датчиков
     blue_average = 0;
     for (int i = 0; i < 10; i++) {  // для считывания результатов 10 измерений для повышения надёжности
        RGB color = colorSensor.colorRead(); // считываю новые данные в объект RGB
        red_average  += color.red;
        green_average += color.green;
        blue_average += color.blue;
      }
     red_average = red_average / 10;  // делю на 10 для нахождения среднего значения измерений
     green_average = green_average / 10;
     blue_average = blue_average / 10;
     // если преобладает какой-либо цвет, то записываю в массив и вывожу в консоль
     if (green_average > color_value){ 
      Serial.print("1");
      board[2][2] = 1;
     }
     else if (red_average > color_value){
      Serial.print("0");
      board[2][2] = 0;
     }
     else if (blue_average > color_value){
      Serial.print("2");
      board[2][2] = 2;
     }
     Serial.print(red_average);
     Serial.print(' ');
     Serial.print(green_average);
     Serial.print(' ');
     Serial.print(blue_average);
     Serial.print(' ');
     
     Serial.println('\t');
     Serial.println('\t');

     servo_0.write(0);  // доводка поля до карйнего дальнего положения 
     delay (800);
     servo_0.write(servo_0_stop_speed);

     digitalWrite(LIGHT_SENSOR_0_PIN,LOW); // подсветка датчиков вкл
     digitalWrite(LIGHT_SENSOR_1_PIN,LOW);
     digitalWrite(LIGHT_SENSOR_2_PIN,LOW);
  }
  
  void makeStep(int step_i, int step_j){ // на вход поступет координата в которую нужно положить шарик
    // поле должно находиться в крайнем дальнем положении(как после scanBoard)
  if (step_i == 0){
    servo_0.write(180);
  delay (1600);
  servo_0.write(servo_0_stop_speed);
  delay(100);
    if (step_j == 0){
      servo_0_mini.write(180);
      delay(500);
      servo_0_mini.write(0);
    }
    else if (step_j == 1){
      servo_1_mini.write(180);
      delay(500);
      servo_1_mini.write(0);
    }
    else if (step_j == 2){
      servo_2_mini.write(180);
      delay(500);
      servo_2_mini.write(0);
    }
   servo_0.write(180);
   delay (1850);
   servo_0.write(servo_0_stop_speed);
  }
  else if (step_i == 1){
    servo_0.write(180);
  delay (800);
  servo_0.write(servo_0_stop_speed);
  delay(100);
    if (step_j == 0){
      servo_0_mini.write(180);
      delay(500);
      servo_0_mini.write(0);
    }
    else if (step_j == 1){
      servo_1_mini.write(180);
      delay(500);
      servo_1_mini.write(0);
    }
    else if (step_j == 2){
      servo_2_mini.write(180);
      delay(500);
      servo_2_mini.write(0);
    }
   servo_0.write(180);
   delay (2640);
   servo_0.write(servo_0_stop_speed);
  }
  else if (step_i == 2){
  servo_0.write(servo_0_stop_speed);
    if (step_j == 0){
      servo_0_mini.write(180);
      delay(500);
      servo_0_mini.write(0);
    }
    else if (step_j == 1){
      servo_1_mini.write(180);
      delay(500);
      servo_1_mini.write(0);
    }
    else if (step_j == 2){
      servo_2_mini.write(180);
      delay(500);
      servo_2_mini.write(0);
    }
   servo_0.write(180);
   delay (3240);
   servo_0.write(servo_0_stop_speed);
  }
}

bool checkWin(){  
    // проверка на наличие победной комбинации из 3х на поле
    if (board[0][0] == 1 and board[0][1] == 1 and board[0][2] == 1) return true;
    if (board[1][0] == 1 and board[1][1] == 1 and board[1][2] == 1) return true;
    if (board[2][0] == 1 and board[2][1] == 1 and board[2][2] == 1) return true;
    
    if (board[0][0] == 1 and board[1][0] == 1 and board[2][0] == 1) return true;
    if (board[0][1] == 1 and board[1][1] == 1 and board[2][1] == 1) return true;
    if (board[0][2] == 1 and board[1][2] == 1 and board[2][2] == 1) return true;
    
    if (board[0][0] == 1 and board[1][1] == 1 and board[2][2] == 1) return true;
    if (board[0][2] == 1 and board[1][1] == 1 and board[2][0] == 1) return true;
    
    if (board[0][0] == 2 and board[0][1] == 2 and board[0][2] == 2) return true;
    if (board[1][0] == 2 and board[1][1] == 2 and board[1][2] == 2) return true;
    if (board[2][0] == 2 and board[2][1] == 2 and board[2][2] == 2) return true;
    
    if (board[0][0] == 2 and board[1][0] == 2 and board[2][0] == 2) return true;
    if (board[0][1] == 2 and board[1][1] == 2 and board[2][1] == 2) return true;
    if (board[0][2] == 2 and board[1][2] == 2 and board[2][2] == 2) return true;
    
    if (board[0][0] == 2 and board[1][1] == 2 and board[2][2] == 2) return true;
    if (board[0][2] == 2 and board[1][1] == 2 and board[2][0] == 2) return true;
    if (board[0][0] != 0 and board[0][1] != 0 and board[0][2] != 0 and board[1][0] != 0 and board[1][1] != 0 and board[1][2] != 0 and board[2][0] != 0 and board[2][1] != 0 and board[2][2] != 0) return true;
    return false;
}

void makeRandomStep() { 
    // делает шаг в случайную клетку на поле
    while (1) {  // цикл для подбора рандомных чисел
      randomSeed(analogRead(A0));
      int random_i = random(3); 
      int random_j = random(3);
      if (board[random_i][random_j] == 0){ // если клетка пустая, то подходит
        step_pos[0] = random_i;
        step_pos[1] = random_j;  // хожу в клетку
        break;
      }
    }
  }

void closeLineOfTwo() { 
    // если стоят 2 подряд фигуры игрока, то предотвращаем победу 
    if ((board[0][1] == 2 and board[0][2] == 2 or board[1][0] == 2 and board[2][0] == 2 or board[1][1] == 2 and board[2][2] == 2) and board[0][0] == 0) {
      step_pos[0] = 0;
      step_pos[1] = 0;
      }
    else if ((board[1][1] == 2 and board[2][1] == 2 or board[0][0] == 2 and board[0][2] == 2) and board[0][1] == 0){
      step_pos[0] = 0;
      step_pos[1] = 1;
    }
   else if ((board[0][0] == 2 and board[0][1] == 2 or board[1][2] == 2 and board[2][2] == 2 or board[1][1] == 2 and board[2][0] == 2) and board[0][2] == 0) {
      step_pos[0] = 0;
      step_pos[1] = 2;
      }
   else if ((board[1][1] == 2 and board[1][2] == 2 or board[0][0] == 2 and board[2][0] == 2) and board[1][0] == 0){
      step_pos[0] = 1;
      step_pos[1] = 0;
      }
    else if ((board[1][0] == 2 and board[1][2] == 2 or board[0][0] == 2 and board[2][2] == 2 or board[0][1] == 2 and board[2][1] == 2 or board[0][2] == 2 and board[2][0] == 2) and board[1][1] == 0){
      step_pos[0] = 1;
      step_pos[1] = 1;
      }
    else if ((board[1][0] == 2 and board[1][1] == 2 or board[0][2] == 2 and board[2][2] == 2) and board[1][2] == 0){
      step_pos[0] = 1;
      step_pos[1] = 2;
      }
    else if ((board[2][1] == 2 and board[2][2] == 2 or board[1][0] == 2 and board[0][0] == 2 or board[1][1] == 2 and board[0][2] == 2) and board[2][0] == 0){
      step_pos[0] = 2;
      step_pos[1] = 0;
      }
    else if ((board[1][1] == 2 and board[0][1] == 2 or board[2][0] == 2 and board[2][2] == 2) and board[2][1] == 0){
      step_pos[0] = 2;
      step_pos[1] = 1;
      }
    else if ((board[2][0] == 2 and board[2][1] == 2 or board[0][2] == 2 and board[1][2] == 2 or board[0][0] == 2 and board[1][1] == 2)and board[2][2] == 0){
      step_pos[0] = 2;
      step_pos[1] = 2;
      }

     
     else if ((board[0][1] == 1 and board[0][2] == 1 or board[1][0] == 1 and board[2][0] == 1 or board[1][1] == 1 and board[2][2] == 1) and board[0][0] == 0){
      step_pos[0] = 0;
      step_pos[1] = 0;
      }
    else if ((board[1][1] == 1 and board[2][1] == 1 or board[0][0] == 1 and board[0][2] == 1) and board[0][1] == 0){
      step_pos[0] = 0;
      step_pos[1] = 1;
    }
   else if ((board[0][0] == 1 and board[0][1] == 1 or board[1][2] == 1 and board[2][2] == 1 or board[1][1] == 1 and board[2][0] == 1) and board[0][2] == 0){
      step_pos[0] = 0;
      step_pos[1] = 2;
      }
   else if ((board[1][1] == 1 and board[1][2] == 1 or board[0][0] == 1 and board[2][0] == 1) and board[1][0] == 0){
      step_pos[0] = 1;
      step_pos[1] = 0;
      }
    else if ((board[1][0] == 1 and board[1][2] == 1 or board[0][0] == 1 and board[2][2] == 1 or board[0][1] == 1 and board[2][1] or board[0][2] == 2 and board[2][0]) and board[1][1] == 0){
      step_pos[0] = 1;
      step_pos[1] = 1;
      }
    else if ((board[1][0] == 1 and board[1][1] == 1 or board[0][2] == 1 and board[2][2] == 1) and board[1][2] == 0){
      step_pos[0] = 1;
      step_pos[1] = 2;
      }
    else if ((board[2][1] == 1 and board[2][2] == 1 or board[1][0] == 1 and board[0][0] == 1 or board[1][1] == 1 and board[0][2] == 1) and board[2][0] == 0){
      step_pos[0] = 2;
      step_pos[1] = 0;
      }
    else if ((board[1][1] == 1 and board[0][1] == 1 or board[2][0] == 1 and board[2][2] == 1) and board[2][1] == 0){
      step_pos[0] = 2;
      step_pos[1] = 1;
      }
    else if ((board[2][0] == 1 and board[2][1] == 1 or board[0][2] == 1 and board[1][2] == 1 or board[0][0] == 1 and board[1][1] == 1) and board[2][2] == 0){
      step_pos[0] = 2;
      step_pos[1] = 2;
      }
    }
  
