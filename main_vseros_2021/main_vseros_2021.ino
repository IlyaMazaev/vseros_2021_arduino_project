#include <TroykaI2CHub.h>

// библиотека для работы с датчиком цвета
#include <TroykaColorSensor.h>
#include <Servo.h>
# define mini_servo_0_pin 13
# define mini_servo_1_pin 12
# define mini_servo_2_pin 11

# define servo_0_pin 3
# define servo_0_stop_speed 89

Servo servo_0_mini;
Servo servo_1_mini;
Servo servo_2_mini;

Servo servo_0;

#define BUTTON_PIN  10

#define color_value 230  // нужно для логики определения цветов
// если например color.blue > color_value , то я считаю, что показание датчика = синий цвет

// пин подсветки светодиода
#define LIGHT_SENSOR_0_PIN   5 
#define LIGHT_SENSOR_1_PIN   6
#define LIGHT_SENSOR_2_PIN   7

#define SENSOR_0_HUB_PIN   0 
#define SENSOR_1_HUB_PIN   2
#define SENSOR_2_HUB_PIN   6

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
  
  servo_0.attach(servo_0_pin);
  servo_0.write(servo_0_stop_speed);

  servo_0_mini.write(180);
  servo_1_mini.write(180);
  servo_2_mini.write(180);
  
  pinMode(BUTTON_PIN, INPUT);
 // инициализируем датчик
 colorSensor.begin();
 // открываем Serial-порт
 Serial.begin(9600);

analogWrite(LIGHT_SENSOR_0_PIN,100); // подсветка датчиков вкл
analogWrite(LIGHT_SENSOR_1_PIN,100);
analogWrite(LIGHT_SENSOR_2_PIN,100);



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
  
  if (color_0.red == 0 and color_0.green == 0 and color_0.blue == 0) Serial.println(0);
  if (color_1.red == 0 and color_1.green == 0 and color_1.blue == 0) Serial.println(1);
  if (color_2.red == 0 and color_2.green == 0 and color_2.blue == 0) Serial.println(2);

  if (color_0.red != 0 and color_0.green != 0 and color_0.blue != 0 and color_1.red != 0 and color_1.green != 0 and color_1.blue != 0 and color_2.red != 0 and color_2.green != 0 and color_2.blue != 0) break;
}
analogWrite(LIGHT_SENSOR_0_PIN,0); // подсветка датчиков выкл
analogWrite(LIGHT_SENSOR_1_PIN,0);
analogWrite(LIGHT_SENSOR_2_PIN,0);
 
}



void loop() {
  servo_0_mini.write(180);
  servo_1_mini.write(180);
  servo_2_mini.write(180);
  
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
   servo_0.write(0);  // если есть победа, то поле возвращается в крайнее ближнее положение
   delay (3240);
   servo_0.write(servo_0_stop_speed);
   servo_0.write(180);
   delay (200);
   servo_0.write(servo_0_stop_speed);
   servo_0.write(0);
   delay (200);
   servo_0.write(servo_0_stop_speed);
   servo_0.write(180);
   delay (200);
   servo_0.write(servo_0_stop_speed);
   servo_0.write(0);
   delay (200);
   servo_0.write(servo_0_stop_speed);
  }
}



void scanBoard() {  
    // сканирование поля и его перезапись(поле должно находиться в крайнем нижнем положении)
     analogWrite(LIGHT_SENSOR_0_PIN,100); // подсветка датчиков вкл
     analogWrite(LIGHT_SENSOR_1_PIN,100);
     analogWrite(LIGHT_SENSOR_2_PIN,100);

     servo_0.write(180); // двигаю 0 строчку к датчикам
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
     Serial.println('\t');

     servo_0.write(180);  // двигаю следующую строчку
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
     Serial.println('\t');

     servo_0.write(180);  // двигаю следующую строчку
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
     Serial.println('\t');
     Serial.println('\t');

     servo_0.write(180);  // доводка поля до карйнего дальнего положения 
     delay (900);
     servo_0.write(servo_0_stop_speed);

     analogWrite(LIGHT_SENSOR_0_PIN,0); // подсветка датчиков вкл
     analogWrite(LIGHT_SENSOR_1_PIN,0);
     analogWrite(LIGHT_SENSOR_2_PIN,0);
  }
  
  void makeStep(int step_i, int step_j){ // на вход поступет координата в которую нужно положить шарик
    // поле должно находиться в крайнем дальнем положении(как после scanBoard)
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
  
