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
#define M_FRIEND "8"
#define M_ENEMY "~"


const int sizeX = 51;
const int sizeY = 21;

//COORD characterPrev = {1, 1};
//COORD character = {1, 1};

struct entity {
	COORD curr;
	COORD prev;
};

entity character = {
		{1, 1},
		{1, 1}
};

short scene[sizeX][sizeY];

void renderCharacter() {
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), character.prev);
	cout << " ";
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), character.curr);
	cout << M_CHARACTER;
	character.prev = character.curr;
};

bool isPath(entity a) {
	short tile = scene[a.curr.X][a.curr.Y];
	if(tile == PATH) return true;
	return false;
};

void undoCurrentMove(entity a) {
	a.curr.X = a.prev.X;
	a.curr.Y = a.prev.Y;
};

void left(entity a) {
	a.curr.X -= 1;
	if(!isPath(a)) undoCurrentMove(a);
	renderCharacter();
};
void right(entity a) {
	a.curr.X += 1;
	if(!isPath(a)) undoCurrentMove(a);
	renderCharacter();
};
void up(entity a) {
	a.curr.Y -= 1;
	if(!isPath(a)) undoCurrentMove(a);
	renderCharacter();
};
void down(entity a) {
	a.curr.Y += 1;
	if(!isPath(a)) undoCurrentMove(a);
	renderCharacter();
};

int run()
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
                case KB_W: //W
                	up(character);
                	break;
                case KB_A: //A
                	left(character);
                	break;
                case KB_S: //S
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

void generateFloor() {
	short nextPts[4][2] = {
			{0, 2},
			{0, -2},
			{2, 0},
			{-2, 0}
	};
	default_random_engine gen;
	uniform_real_distribution<float> dist(0, 1);
	COORD c = character.curr;
	COORD n = c;
	bool done = false;
	vector<COORD> next;
	while (!done) {
		for(int i = 0; i < 4; i++) {
			short pt[2] = {nextPts[i][0], nextPts[i][1]};
			if(!((c.Y+pt[1] > sizeY) || (c.X+pt[0] > sizeX)) && scene[c.X+pt[0]][c.Y+pt[1]] == UNMADE) {
				n = {c.X+pt[0], c.Y+pt[1]};
				next.push_back(n);
			}
		}
	}
};

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
	//testRender();
	renderCharacter();
	run();
};

int width = 0, height = 0;

unsigned char* readBMP(char* filename)
{
    int i;
    FILE* f = fopen(filename, "rb");
    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

    // extract image height and width from header
    width = *(int*)&info[18];
    height = *(int*)&info[22];

    int size = 1 * width * height;
    unsigned char* data = new unsigned char[size]; // allocate 3 bytes per pixel
    fread(data, sizeof(unsigned char), size, f); // read the rest of the data at once
    fclose(f);

    for(i = 0; i < size; i += 3)
    {
            unsigned char tmp = data[i];
            data[i] = data[i+2];
            data[i+2] = tmp;
    }

    return data;
}

void read2() {
	FILE* f = fopen(".\\Maps\\test2.bmp", "rb");

	unsigned char info[54];
	fread(info, sizeof(unsigned char), 54, f);

	width = (int)info[18];
	height = (int)info[22];

	int size = width * height;
	unsigned char data[size];
	fread(data, sizeof(unsigned char), size, f);
	fclose(f);

	cout << width << endl;
	cout << height << endl;
	for (int i = 0; i < width; i++) {
		for(int j = 0; j < height; j++) {
			cout << bitset<8>(data[i]);
		}
		cout << endl;
	}

};

int main() {
	system("cls");
	start();
	//read2();

	return 0;
};

