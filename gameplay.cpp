#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
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

struct Score {
    string name;
    int score;
} scores[100], tempScore;

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
void saveScore();
bool isOver();

void play();
void high();
void sortScore(int);

int maps[34][15], tempMap[34][15];
int iObject = 0, typeObject, nextType = random(0, 6), score = 0;
int posX = object[iObject].pos.x, posY = object[iObject].pos.y;
string playerName, scoreLists[1000];

const int SPACE = 0, BLOCK = 1, WALL = 2, PERM_BLOCK = 3;

int main()
{
  clear();

  string logo[] = {
    " __________________________________________________________________ ",
    "|                                                                  |",
    "|  __________   ________   __________   _______    __   ________   |",
    "| |___    ___| |   _____| |___    ___| |   __  |  |  | |   _____|  |",
    "|     |  |     |  |_____      |  |     |  |__| |  |  | |  |_____   |",
    "|     |  |     |   _____|     |  |     |     __|  |  | |_____   |  |",
    "|     |  |     |  |           |  |     |  |  \\    |  |       |  |  |",
    "|     |  |     |  |_____      |  |     |  |\\  \\   |  |  _____|  |  |",
    "|     |__|     |________|     |__|     |__| \\__\\  |__| |________|  |",
    "|                                                                  |",
    "|                   Oleh : Ansori, Yuri, Fifi                      |",
    "|__________________________________________________________________|"
  };

  for (int i = 0; i < 12; i++) {
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
  clear();
  generateMap();

  while (true) {
    clear();
    updateMap();

    cout << endl << "Score : " << score << endl;
    cout << endl << "Bentuk Selanjutnya : " << endl;

    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        if (object[iObject].shape[nextType][i][j] == BLOCK) {
          cout << "[]";
        } else {
          cout << "  ";
        }
      }
      cout << endl;
    }

    if (isOver()) break;
    control();
  }

  cout << endl << "Game Over !";
  saveScore();

  cout << endl << "Apakah ingin main lagi ( y / t ) ? ";
  cin >> choice;

  if (!cin.fail() && choice == 'y') {
    play();
  } else {
    main();
  }
}

void high()
{
  clear();

  string line;
  int count = 0;
  ifstream myfile ("score.dat");

  cout << "High Score : " << endl << endl;

  // Copy score.dat value into Struct Score
  if (myfile.is_open())
  {
    while (getline (myfile, line))
    {
      int index = 0, tempInt;
      string tempString;
      scores[count].name = "";

      for (int i = 0; i < line.length(); i++) {
        if (line[i] == '_') {
          ++index;
          continue;
        }

        if (index == 0) {
          scores[count].name += toupper(line[i]);
        } else {
          tempString += line[i];
          stringstream num(tempString);
          num >> tempInt;
          scores[count].score = tempInt;
        }
      }

      ++count;
    }

    myfile.close();
  }

  sortScore(count);

  // Show Top 10
  for (int i = 0; i < 10; i++) {
    if (scores[i].name != "") {
      cout << i + 1 << ". " << scores[i].name << " " << scores[i].score << endl;
    }
  }

  cin.ignore();
  cout << endl << "Tekan sembarang tombol untuk kembali...";
  getch();
  main();
}

// Sorting Score from Highest to Lowest
void sortScore(int count)
{
  bool sorted = true;

  do {
    sorted = true;

    for (int i = 0; i < count - 1; i++) {
      if (scores[i].score < scores[i + 1].score) {
        tempScore.name = scores[i].name;
        tempScore.score = scores[i].score;

        scores[i].name = scores[i + 1].name;
        scores[i].score = scores[i + 1].score;
        scores[i + 1].name = tempScore.name;
        scores[i + 1].score = tempScore.score;

        sorted = false;
      }
    }
  } while(!sorted);

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
        maps[i][j] = WALL;
        tempMap[i][j] = WALL;
      } else {
        if (j > 0 && j < WIDTH - 1) {
          maps[i][j] = SPACE;
          tempMap[i][j] = SPACE;
        } else {
          maps[i][j] = WALL;
          tempMap[i][j] = WALL;
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
    typeObject = nextType;
    nextType = random(0, 6);
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

        if (object[iObject].shape[typeObject][iShape][jShape] == BLOCK) {
          tempMap[i][j] = BLOCK;
        } else {
          tempMap[i][j] = maps[i][j];
        }

        ++jShape;
      } else {
        tempMap[i][j] = maps[i][j];
      }

      switch (tempMap[i][j]) {
        case SPACE: cout << "  "; break;
        case BLOCK: cout << "[]"; break;
        case WALL: cout << "**"; break;
        case PERM_BLOCK: cout << "[]"; break;
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
      if (tempMap[i][j] == BLOCK) {
        maps[i][j] = PERM_BLOCK;
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

       if (tempMap[i][j] == BLOCK && num == WALL | num == PERM_BLOCK) {
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
      if (maps[i][j] == PERM_BLOCK) {
        ++count;
      }
    }

    if (count == 13) {
      clearRow(i);
      fall(i);
      score += 25;
    } else {
      --i;
    }
  }
}

// to Clean One Row
void clearRow(int index)
{
  for (int i = 0; i < WIDTH; i++) {
    if (maps[index][i] == PERM_BLOCK) {
      maps[index][i] = SPACE;
    }
  }
}

void fall(int index)
{
  for (int i = index; i >= 0; i--) {
    for (int j = 0; j < WIDTH; j++) {
      if (maps[i - 1][j] == PERM_BLOCK) {
        maps[i][j] = PERM_BLOCK;
        maps[i - 1][j] = SPACE;
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

void saveScore()
{
  ofstream out;

  out.open("score.dat", ios_base::app);
  out << playerName << "_" << score << endl;
  out.close();
}

bool isOver()
{
  for (int i = 0; i < WIDTH; i++) {
    if (maps[4][i] == PERM_BLOCK) {
      return true;
    }
  }

  return false;
}
