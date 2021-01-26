void setup() { 

  int board[3][3] = { // создание доски для новой игры
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}
  };
  
  void makeRandomStep(); { 
    // делает шаг в случайную клетку на поле
    while (1) {  // цикл для подбора рандомных чисел
      randomSeed(analogRead(A0));
      int random_i = random(3); 
      int random_j = random(3);
      if (board[random_i][random_j] == 0){ // если клетка пустая, то подходит
        board[random_i][random_j] = 2;  // хожу в клетку
        break;
      }
    }
  }

  void closeLineOfTwo(); { 
    // если стоят 2 подряд фигуры игрока, то предотвращаем победу 
    if ((board[0][1] == 2) and (board[0][2] == 2)) or ((board[1][0] == 2) and (board[2][0] == 2)) or ((board[1][1] == 2) and (board[2][2] == 2)) board[0][0] = 1;
    if ((board[1][1] == 2) and (board[2][1] == 2)) board[0][1] = 1;
    if ((board[0][0] == 2) and (board[0][1] == 2)) or ((board[1][2] == 2) and (board[2][2] == 2)) or ((board[1][1] == 2) and (board[2][0] == 2)) board[0][2] = 1;
    if ((board[1][1] == 2) and (board[1][2] == 2)) board[1][0] = 1;
    if ((board[1][0] == 2) and (board[1][1] == 2) board[1][2] = 1;
    if ((board[2][1]) == 2 and (board[2][2] == 2)) or ((board[1][0] == 2) and (board[0][0] == 2)) or ((board[1][1] == 2) and (board[0][2] == 2)) board[2][0] = 1;
    if ((board[1][1] == 2) and (board[0][1] == 2)) board[2][1] = 1;
    if ((board[2][0] == 2) and (board[2][1] == 2)) or ((board[0][2] == 2) and (board[1][2] == 2)) or ((board[0][0] == 2) and (board[1][1] == 2)) board[2][2] = 1;
  }

  
  bool checkWin();{  
    // проверка на наличие победной комбинации из 3х на поле
    if (board[0] == {1, 1, 1}) or (board[1] == {1, 1, 1}) or (board[2] == {1, 1, 1}) return true;
    if (board[0][0] == 1) and (board[1][0] == 1) and (board[2][0] == 1) return true;
    if (board[0][1] == 1) and (board[1][1] == 1) and (board[2][1] == 1) return true;
    if (board[0][2] == 1) and (board[1][2] == 1) and (board[2][2] == 1) return true;
    
    if (board[0][0] == 1) and (board[1][1] == 1) and (board[2][2] == 1) return true;
    if (board[0][2] == 1) and (board[1][1] == 1) and (board[2][0] == 1) return true;
    
    if (board[0] == {2, 2, 2}) or (board[1] == {2, 2, 2}) or (board[1] == {2, 2, 2}) return true;
    if (board[0][0] == 2) and (board[1][0] == 2) and (board[2][0] == 2) return true;
    if (board[0][1] == 2) and (board[1][1] == 2) and (board[2][1] == 2) return true;
    if (board[0][2] == 2) and (board[1][2] == 2) and (board[2][2] == 2) return true;
    
    if (board[0][0] == 2) and (board[1][1] == 2) and (board[2][2] == 2) return true;
    if (board[0][2] == 2) and (board[1][1] == 2) and (board[2][0] == 2) return true;
    return false;  
  }
}

void loop() {
  int board[3][3] = { // создание доски для новой игры
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}
  };
}
