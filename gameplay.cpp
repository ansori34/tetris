#include <iostream>
#include <fstream>

#include "linux.h"

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

int maps[34][15] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
  { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
  { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
  { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
  { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
  { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
  { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
  { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
  { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
  { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
  { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
  { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
  { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
  { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
  { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
  { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
  { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
  { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
  { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
  { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
  { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
  { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
  { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
  { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
  { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
  { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
  { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
  { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
  { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
  { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 }
};

int mapWidth = sizeof(maps[0]) / sizeof(maps[0][0]);
int mapHeight = sizeof(maps) / sizeof(maps[0]);

int random(int, int);
void updateMap();
void capture();
void control();
bool checkSide(char);
void checkScore();
void clearRow(int);
void fall(int);
void rotate(int (&shape)[4][4]);
bool isOver();

int tempMap[34][15], iObject = 0, typeObject = random(0, 6), score = 0;
bool spawn = false;

int main()
{
  // string line;
  // ifstream myfile ("data.dat");
  // if (myfile.is_open())
  // {
  //   while ( getline (myfile, line) )
  //   {
  //     cout << line << '\n';
  //   }
  //   myfile.close();
  // }

  for (int i = 0; i < mapHeight; i++) {
    for (int j = 0; j < mapWidth; j++) {
      tempMap[i][j] = maps[i][j];
    }
  }

  while (true) {
    system("clear");

    updateMap();

    cout << endl << " ( " << object[iObject].pos.x
      << " , " << object[iObject].pos.y << " )" << endl << endl;

    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        cout << object[iObject].shape[typeObject][i][j] << " ";
      }
      cout << endl;
    }

    if (isOver()) break;
    control();
  }

  cout << endl << "Game Over";
}

int random(int min, int max)
{
  srand(time(NULL));
  return min + (rand() % ( max - min + 1 ));
}

void updateMap()
{
  if (!checkSide('s')) {
    ++object[iObject].pos.y;
  } else {
    capture();
    typeObject = random(0, 6);
    ++iObject;
  }

  checkScore();

  int iShape = 0, jShape = 0, x = object[iObject].pos.x, y = object[iObject].pos.y;
  int shapeWidth = 4, tempI = 0;

  switch (y) {
    case 0: iShape = 3; break;
    case 1: iShape = 2; break;
    case 2: iShape = 1; break;
  }

  for (int i = 4; i < mapHeight; i++) {
    jShape = 0;

    if (x < 0) {
      jShape = -x;
    }

    for (int j = 0; j < mapWidth; j++) {
      if (j >= x && j < x + shapeWidth && i > y && i <= y + shapeWidth) {
      // if (j >= x && j < x + shapeWidth && iShape <= 3) {

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

    if (i > y && i <= y + shapeWidth) {
      ++iShape;
    }

    cout << endl;
  }
}

// to Save Condition
void capture()
{
  for (int i = 0; i < mapHeight; i++) {
    for (int j = 0; j < mapWidth; j++) {
      if (tempMap[i][j] == 1) {
        maps[i][j] = 3;
      }
    }
  }
}

void control()
{
  int posX = object[iObject].pos.x, posY = object[iObject].pos.y;

  // if (object.active) {
    switch (getch()) {
      case 'w': // For Rotate
        rotate(object[iObject].shape[typeObject]);
        break;

      case 's': // For Down
        break;

      case 'a': // For Left
        if (!checkSide('a')) {
          --object[iObject].pos.x;
        }
        break;

      case 'd': // For Right
        if (!checkSide('d')) {
          ++object[iObject].pos.x;
        }
        break;
    }
  // }
}

// to Check Collision between Object
bool checkSide(char type)
{
  int num;
  bool collide = false;

  for (int i = 0; i < mapHeight; i++) {
     for (int j = 0; j < mapWidth; j++) {
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
  for (int i = mapHeight - 1; i >= 0;) {
    int count = 0;

    for (int j = 0; j < mapWidth; j++) {
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
  for (int i = 0; i < mapWidth; i++) {
    if (maps[index][i] == 3) {
      maps[index][i] = 0;
    }
  }
}

void fall(int index)
{
  for (int i = index; i >= 0; i--) {
    for (int j = 0; j < mapWidth; j++) {
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
  if (object[iObject].pos.x <= 0) {
    object[iObject].pos.x = 1;
  } else if (object[iObject].pos.x >= 11) {
    object[iObject].pos.x = 10;
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

bool isOver()
{
  for (int i = 0; i < mapWidth; i++) {
    if (maps[4][i] == 3) {
      return true;
    }
  }

  return false;
}
