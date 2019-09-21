/*
 * test.cpp
 *
 *  Created on: Sep 21, 2019
 *      Author: klempb
 */

#include <windows.h>
#include <iostream>
#include <conio.h>
#include <random>
#include <fstream>
#include <bitset>

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

#define UNMADE -1
#define PATH 0
#define WALL 1

#define M_UNMADE "!"
#define M_WALL "#"
#define M_PATH " "
#define M_CHARACTER "&"
#define M_COMPANION "8"
#define M_ENEMY "~"


const int sizeX = 51;
const int sizeY = 21;

struct entity {
	COORD curr;
	COORD prev;
};

/*
entity afriend;

entity enemies[4];
*/

COORD character_origin = {1, 1};

entity* character = new entity;

short scene[sizeX][sizeY];
/*
void debug(char* a) {
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0, 22});
	cout << a;
};
*/

void initCharacter() {
	character->curr = {1, 1};
	character->prev = {1, 1};
};

void renderCharacter() {
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), character->prev);
	cout << " ";
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), character->curr);
	cout << M_CHARACTER;
	character->prev = character->curr;
};

bool isPath(entity* ent) {
	if(ent->curr.X <= 0 || ent->curr.X >= sizeX || ent->curr.Y <= 0 || ent->curr.Y >= sizeY) {
		return false;
	};
	short tile = scene[ent->curr.X][ent->curr.Y];
	if(tile == PATH) return true;
	return false;
};

void undoCurrentMove(entity *ent) {
	ent->curr.X = ent->prev.X;
	ent->curr.Y = ent->prev.Y;
};

void left(entity* ent) {
	ent->curr.X -= 2;
	if(!isPath(ent)) undoCurrentMove(ent);
	renderCharacter();
};
void right(entity* ent) {
	ent->curr.X += 2;
	if(!isPath(ent)) undoCurrentMove(ent);
	renderCharacter();
};
void up(entity* ent) {
	ent->curr.Y -= 1;
	if(!isPath(ent)) undoCurrentMove(ent);
	renderCharacter();
};
void down(entity* ent) {
	ent->curr.Y += 1;
	if(!isPath(ent)) undoCurrentMove(ent);
	renderCharacter();
};

int run()
{
   int KB_code=0;

   while(KB_code != KB_ESCAPE)
   {
     if (kbhit())
      {
            KB_code = getch();

            switch (KB_code)
            {
                case KB_LEFT:
                	left(character);
                	break;
                case KB_RIGHT:
                	right(character);
                	break;
                case KB_UP:
                	up(character);
                	break;
                case KB_DOWN:
                	down(character);
                	break;
                case KB_W:
                	up(character);
                	break;
                case KB_A:
                	left(character);
                	break;
                case KB_S:
                	down(character);
                	break;
                case KB_D:
                	right(character);
                	break;
            }

      }
  }

  return 0;
};

void generateOuterWall() {
	for (short i = 0; i < (short)sizeX; i++)
		for(short j = 0; j < (short)sizeY; j++)
			if(i == 0 || i == (short)sizeX - 1 || j == 0 || j == (short)sizeY - 1)
				scene[(int)i][(int)j] = WALL;
			else
				scene[(int)i][(int)j] = PATH;
};

/*
void generateCreatures(int nenemies) {
	default_random_engine gen;
	uniform_int_distribution<short> distX(0, sizeX/2);
	uniform_int_distribution<short> distY(0, sizeY);
	for(int i = 0; i < nenemies; i++) {
		COORD a;
		a.X = distX(gen)*2;
		a.Y = distY(gen);
		entity b = {a, a};
		enemies[i] = b;
	}
};
*/

void generate() {
	generateOuterWall();
};

void renderScene() {
	for(short i = 0; i < (short)sizeX; i++) {
		for(short j = 0; j < (short)sizeY; j++) {
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {i, j});
			switch(scene[(int)i][(int)j]) {
			case WALL:
				cout << M_WALL;
				break;
			case UNMADE:
				cout << M_UNMADE;
				break;
			case PATH:
				cout << M_PATH;
				break;
			}
		}
	}
};

void testRender() {
	for(short i = 0; i < (short)sizeX; i++)
		for(short j = 0; j < (short)sizeY; j++)
			if(i % 2 == 0 && j % 2 == 0) {
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {i, j});
				cout << M_UNMADE;
			}
};

void start() {
	generate();
	renderScene();
	initCharacter();
	//testRender();
	renderCharacter();
	run();
};

int main() {
	system("cls");
	start();
	//read2();

	return 0;
};

