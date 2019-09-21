/*
 * test.cpp
 *
 *  Created on: Sep 21, 2019
 *      Author: klempb
 */

#include <windows.h>
#include <iostream>
#include <conio.h>

using namespace std;

#define KB_UP 72
#define KB_DOWN 80
#define KB_LEFT 75
#define KB_RIGHT 77
#define KB_ESCAPE 27
#define KB_W 119
#define KB_A 97
#define KB_S 115
#define KB_D 100

#define PATH 0
#define WALL 1

const int sizeX = 50;
const int sizeY = 20;

COORD characterPrev = {1, 1};
COORD character = {1, 1};

short walls[sizeX][sizeY];

void renderCharacter() {
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), characterPrev);
	cout << " ";
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), character);
	cout << "&";
	characterPrev = character;
};

bool isPath() {
	short tile = walls[character.X][character.Y];
	if(tile == PATH) return true;
	return false;
};

void undoCurrentMove() {
	character.X = characterPrev.X;
	character.Y = characterPrev.Y;
};

void left() {
	character.X -= 1;
	if(!isPath()) undoCurrentMove();
	renderCharacter();
};
void right() {
	character.X += 1;
	if(!isPath()) undoCurrentMove();
	renderCharacter();
};
void up() {
	character.Y -= 1;
	if(!isPath()) undoCurrentMove();
	renderCharacter();
};
void down() {
	character.Y += 1;
	if(!isPath()) undoCurrentMove();
	renderCharacter();
};

int input()
{
   int KB_code=0;

   while(KB_code != KB_ESCAPE )
   {
     if (kbhit())
      {
            KB_code = getch();

            switch (KB_code)
            {
                case KB_LEFT:
                	left();
                	break;
                case KB_RIGHT:
                	right();
                	break;
                case KB_UP:
                	up();
                	break;
                case KB_DOWN:
                	down();
                	break;
                case KB_W: //W
                	up();
                	break;
                case KB_A: //A
                	left();
                	break;
                case KB_S: //S
                	down();
                	break;
                case KB_D:
                	right();
                	break;
            }

      }
  }

  return 0;
}

int main() {
	//printf("\033c");
	system("cls");
	COORD c = { 22, 22 };
	for (short i = 0; i < 50; i++) {
		for(short j = 0; j < 20; j++) {
			if(i == 0 || i == 49 || j == 0 || j == 19) {
				c = {i, j};
				walls[i][j] = WALL;
			};
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
			cout << "#";
		}
	}
	renderCharacter();
	input();

	return 0;
};

