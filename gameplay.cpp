#include <iostream>
#include <fstream>
#include "linux.h"
#define WIDTH 15
#define HEIGHT 34

using namespace std;

struct Position {
  int x;
  int y;
};

struct Object {
  Position pos = { 7, 1 };
  int shape[7][4][4] = {
    {
      { 0, 0, 0, 0 },
      { 0, 1, 1, 0 },
      { 0, 1, 1, 0 },
      { 0, 0, 0, 0 }
    },
    {
      { 0, 0, 0, 0 },
      { 1, 1, 1, 1 },
      { 0, 0, 0, 0 },
      { 0, 0, 0, 0 }
    },
    {
      { 0, 0, 0, 0 },
      { 0, 0, 1, 1 },
      { 0, 1, 1, 0 },
      { 0, 0, 0, 0 }
    },
    {
      { 0, 0, 0, 0 },
      { 0, 1, 1, 0 },
      { 0, 0, 1, 1 },
      { 0, 0, 0, 0 }
    },
    {
      { 0, 0, 0, 0 },
      { 0, 1, 1, 1 },
      { 0, 1, 0, 0 },
      { 0, 0, 0, 0 }
    },
    {
      { 0, 0, 0, 0 },
      { 0, 1, 1, 1 },
      { 0, 0, 0, 1 },
      { 0, 0, 0, 0 }
    },
    {
      { 0, 0, 0, 0 },
      { 0, 1, 1, 1 },
      { 0, 0, 1, 0 },
      { 0, 0, 0, 0 }
    }
  };
} object[1000];

int random(int, int);
void generateMap();
void updateMap();
void capture();
void control();
bool checkSide(char);
void checkScore();
void clearRow(int);
void fall(int);
void rotate(int (&shape)[4][4]);
void saveScore(string);
bool isOver();

void play();
void high();

int maps[34][15], tempMap[34][15];
int iObject = 0, typeObject = random(0, 6), score = 0;
int posX = object[iObject].pos.x, posY = object[iObject].pos.y;
bool spawn = false;
string playerName;

int main()
{
  system("clear");

  string logo[] = {
    " __________________________________________________________________ ",
    "|  __________   ________   __________   _______    __   ________   |",
    "| |___    ___| |   _____| |___    ___| |   __  |  |  | |   _____|  |",
    "|     |  |     |  |_____      |  |     |  |__| |  |  | |  |_____   |",
    "|     |  |     |   _____|     |  |     |     __|  |  | |_____   |  |",
    "|     |  |     |  |           |  |     |  |  \\    |  |       |  |  |",
    "|     |  |     |  |_____      |  |     |  |\\  \\   |  |  _____|  |  |",
    "|     |__|     |________|     |__|     |__| \\__\\  |__| |________|  |",
    "|__________________________________________________________________|"
  };

  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 69; j++) {
      cout << logo[i][j];
    }

    cout << endl;
  }

  cout << endl << "Menu Utama : " << endl;
  cout << "1. Mainkan" << endl;
  cout << "2. High Score" << endl;

  while (true) {
    int pilih;

    cout << endl << "Pilihan = ";
    cin >> pilih;

    if (cin.fail()) {
      cin.clear();
      cin.ignore();
      cout << "Input Salah !" << endl;
      continue;
    }

    if (pilih == 1) {
      cout << endl << "Nama Anda = ";
      cin.ignore();
      getline(cin, playerName);
      play();
      break;
    } else if (pilih == 2) {
      high();
      break;
    } else {
      cout << "Input Salah !" << endl;
    }
  }
}

void play()
{
  char choice;
  system("clear");

  generateMap();

  while (true) {
    system("clear");

    updateMap();
    cout << endl << "Score : " << score;
    cout << endl << endl;

    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        cout << object[iObject].shape[typeObject][i][j] << " ";
      }
      cout << endl;
    }

    if (isOver()) break;
    control();
  }

  cout << endl << "Game Over !";
  saveScore(playerName);

  cout << endl << "Apakah ingin main lagi (y/n) ? ";
  cin >> choice;

  if (!cin.fail() && choice == 'y') {
    play();
  } else {
    main();
  }
}

void high()
{
  system("clear");
  string line;
  ifstream myfile ("score.dat");

  cout << "High Score : " << endl << endl;

  if (myfile.is_open())
  {
    while (getline (myfile, line))
    {
      cout << line << endl;
    }

    myfile.close();
  }

  cin.ignore();
  cout << endl << "Tekan sembarang tombol untuk kembali...";
  getch();
  main();
}

int random(int min, int max)
{
  srand(time(NULL));
  return min + (rand() % ( max - min + 1 ));
}

void generateMap()
{
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {

      if (i == HEIGHT - 1) {
        maps[i][j] = 2;
        tempMap[i][j] = 2;
      } else {
        if (j > 0 && j < WIDTH - 1) {
          maps[i][j] = 0;
          tempMap[i][j] = 0;
        } else {
          maps[i][j] = 2;
          tempMap[i][j] = 2;
        }
      }

    }
  }
}

void updateMap()
{
  if (!checkSide('s')) {
    ++posY;
  } else {
    capture();
    typeObject = random(0, 6);
    ++iObject;
    posX = object[iObject].pos.x;
    posY = object[iObject].pos.y;
  }

  checkScore();

  int iShape = 0, jShape = 0, shapeWidth = 4;

  switch (posY) {
    case 0: iShape = 3; break;
    case 1: iShape = 2; break;
    case 2: iShape = 1; break;
  }

  for (int i = 4; i < HEIGHT; i++) {
    jShape = 0;

    if (posX < 0) {
      jShape = -posX;
    }

    for (int j = 0; j < WIDTH; j++) {
      if (j >= posX && j < posX + shapeWidth && i > posY && i <= posY + shapeWidth) {

        if (object[iObject].shape[typeObject][iShape][jShape] == 1) {
          tempMap[i][j] = 1;
        } else {
          tempMap[i][j] = maps[i][j];
        }

        ++jShape;
      } else {
        tempMap[i][j] = maps[i][j];
      }

      switch (tempMap[i][j]) {
        case 0: cout << "  "; break;
        case 1: cout << "[]"; break;
        case 2: cout << "**"; break;
        case 3: cout << "[]"; break;
      }

      // cout << tempMap[i][j];
    }

    if (i > posY && i <= posY + shapeWidth) {
      ++iShape;
    }

    cout << endl;
  }
}

// to Save Condition
void capture()
{
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      if (tempMap[i][j] == 1) {
        maps[i][j] = 3;
      }
    }
  }
}

void control()
{
  switch (getch()) {
    case 'w': // For Rotate
      rotate(object[iObject].shape[typeObject]);
      break;

    case 'a': // For Left
      if (!checkSide('a')) {
        --posX;
      }
      break;

    case 'd': // For Right
      if (!checkSide('d')) {
        ++posX;
      }
      break;
  }
}

// to Check Collision between Object
bool checkSide(char type)
{
  int num;
  bool collide = false;

  for (int i = 0; i < HEIGHT; i++) {
     for (int j = 0; j < WIDTH; j++) {
       switch (type) {
         case 's': num = tempMap[i + 1][j]; break;
         case 'a': num = tempMap[i][j - 1]; break;
         case 'd': num = tempMap[i][j + 1]; break;
       }

       if (tempMap[i][j] == 1 && num == 2 | num == 3) {
         collide = true;
       }
     }
  }

  return collide;
}

void checkScore()
{
  for (int i = HEIGHT - 1; i >= 0;) {
    int count = 0;

    for (int j = 0; j < WIDTH; j++) {
      if (maps[i][j] == 3) {
        ++count;
      }
    }

    if (count == 13) {
      clearRow(i);
      fall(i);
      ++score;
    } else {
      --i;
    }
  }
}

// to Clean One Row
void clearRow(int index)
{
  for (int i = 0; i < WIDTH; i++) {
    if (maps[index][i] == 3) {
      maps[index][i] = 0;
    }
  }
}

void fall(int index)
{
  for (int i = index; i >= 0; i--) {
    for (int j = 0; j < WIDTH; j++) {
      if (maps[i - 1][j] == 3) {
        maps[i][j] = 3;
        maps[i - 1][j] = 0;
      }
    }
  }
}

// to Rotate Object
void rotate(int (&shape)[4][4])
{
  if (posX <= 0) {
    posX = 1;
  } else if (posX >= 11) {
    posX = 10;
  }

  int temp[4][4];

  // Rotate Value
  for (int i = 0; i < 4; i++) {
    int tempJ = 0;
    for (int j = 3; j >= 0; j--) {
      temp[i][tempJ] = shape[j][i];
      ++tempJ;
    }
  }

  // Change Value
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      shape[i][j] = temp[i][j];
    }
  }
}

void saveScore(string name)
{
  ofstream out;

  out.open("score.dat", ios_base::app);
  out << name << "_" << score << endl;
  out.close();
}

bool isOver()
{
  for (int i = 0; i < WIDTH; i++) {
    if (maps[4][i] == 3) {
      return true;
    }
  }

  return false;
}
