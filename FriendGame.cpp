/*
 * FriendGame.cpp
 *
 *  Created on: Sep 21, 2019
 *      Author: Ben Klemp
 *
 *  Say hi to your friend as fast as possible
 *  Avoid talking to others because you are shy
 */

//includes
#include <windows.h>
#include <iostream>
#include <conio.h>
#include <random>
#include <ctime>

//makes my life easier
using namespace std;

//key mappings
#define KB_UP 72
#define KB_DOWN 80
#define KB_LEFT 75
#define KB_RIGHT 77
#define KB_ESCAPE 27
#define KB_W 119
#define KB_A 97
#define KB_S 115
#define KB_D 100
#define KB_N 110
#define KB_Y 121

//tiles
#define UNMADE -1
#define PATH 0
#define WALL 1
//entities
#define UNPLACEABLE -1
#define NONE 0
#define PLAYER 1
#define COMPANION 2
#define STRANGER 3
//models
#define M_UNMADE "!"
#define M_WALL "#"
#define M_PATH " "
#define M_CHARACTER "&"
#define M_COMPANION "8"
#define M_STRANGER "~"
//color
#define C_DEFAULT FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
#define C_WALL C_DEFAULT
#define C_CHARACTER FOREGROUND_GREEN
#define C_COMPANION FOREGROUND_BLUE | FOREGROUND_INTENSITY
#define C_STRANGER FOREGROUND_RED

//board size
const int sizeX = 51;
const int sizeY = 21;
//timer
unsigned long starttime = 0;
//entity setup
const int total_strangers = 4;

struct entity {
	short type;
	COORD curr;
	COORD prev;
};

entity* companion = new entity;

entity* strangers[total_strangers];

COORD character_origin = {1, 1};

entity* character = new entity;
//board
short scene[sizeX][sizeY];
short entities[sizeX][sizeY];
//win flag
bool won = false;
//score keeper
bool first = true;
const int scores = 5;
int highscores[scores];
//pre-definition of functions
void start();
void close();
void win();
int updateScoreboard(int);

//random numbers
default_random_engine gen;
struct timespec mytmhere;
uniform_int_distribution<short> distX(1, sizeX/2-1);
uniform_int_distribution<short> distY(1, sizeY-2);
uniform_real_distribution<float> rnd(0, 1);
uniform_int_distribution<int> getDir(0, 3);

/****** Running Functions ******/

//return the model for the given type of entity
char getModel(short type) {
	switch(type) {
	case PLAYER:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), C_CHARACTER);
		return *M_CHARACTER;
	case STRANGER:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), C_STRANGER);
		return *M_STRANGER;
	case COMPANION:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), C_COMPANION);
		return *M_COMPANION;
	default:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), C_DEFAULT);
		return *M_PATH;
	};
};

//updates the entities registered position
void updateEntity(entity* ent) {
	entities[ent->prev.X][ent->prev.Y] = NONE;
	entities[ent->curr.X][ent->curr.Y] = ent->type;
};

//render the given entity
void renderEntity(entity* ent) {
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), ent->prev);
	cout << " ";
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), ent->curr);
	cout << getModel(ent->type);
	ent->prev = ent->curr;
};

//check whether the entity can move to that location
bool canMove(entity* ent) {
	if(ent->curr.X <= 0 || ent->curr.X >= sizeX || ent->curr.Y <= 0 || ent->curr.Y >= sizeY) {
		return false;
	};
	short tile = scene[ent->curr.X][ent->curr.Y];
	short tile_ent = entities[ent->curr.X][ent->curr.Y];
	//special checks
	if(ent->type == PLAYER && tile_ent == COMPANION) won = true;
	if(ent->type == PLAYER && tile_ent == STRANGER) starttime -= 2500;
	else if (ent->type == STRANGER && tile_ent == PLAYER) starttime -= 1000;
	if(tile == PATH && tile_ent == NONE) return true;
	return false;
};

//reset an entity that tried to move to an invalid location
void undoCurrentMove(entity *ent) {
	ent->curr.X = ent->prev.X;
	ent->curr.Y = ent->prev.Y;
};

//move the entity in the given direction
void left(entity* ent) {
	ent->curr.X -= 2;
	if(!canMove(ent)) undoCurrentMove(ent);
	updateEntity(ent);
	renderEntity(ent);
};
void right(entity* ent) {
	ent->curr.X += 2;
	if(!canMove(ent)) undoCurrentMove(ent);
	updateEntity(ent);
	renderEntity(ent);
};
void up(entity* ent) {
	ent->curr.Y -= 1;
	if(!canMove(ent)) undoCurrentMove(ent);
	updateEntity(ent);
	renderEntity(ent);
};
void down(entity* ent) {
	ent->curr.Y += 1;
	if(!canMove(ent)) undoCurrentMove(ent);
	updateEntity(ent);
	renderEntity(ent);
};

void randomMove(entity* ent) {
	if((bool)round(rnd(gen))) {
		switch(getDir(gen)) {
		case 0:
			left(ent);
			break;
		case 1:
			up(ent);
			break;
		case 2:
			right(ent);
			break;
		case 3:
			down(ent);
			break;
		}
	}
};

void nonPlayers() {
	for(int i = 0; i < total_strangers; i++)
		randomMove(strangers[i]);
	randomMove(companion);
};

//main function for the game, takes keyboard input
void run()
{
   int KB_code=0;

   while(KB_code != KB_ESCAPE)
   {
	   //tick clock
	   if(clock() % 150 == 0) {
		   SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), C_DEFAULT);
		   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0, 22});
		   cout << clock();
		   if(won) {
			   win();
			   break;
		   };
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
		 nonPlayers();
	   }
  }
};

//shows a "you win" message and updates the scoreboard
void win() {
	int a = 100000 - 2.5*(clock() - starttime);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), C_DEFAULT);
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {18, 22});
	cout << "--- YOU WIN! ---";
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {15, 23});
	int i = 0;
	char* ch = new char[45];
	sprintf(ch, "Your score was %i!", a);
	char c = *ch;
	while(c != '\0') {
		c = *(ch + i++);
		cout << c << flush;
		Sleep(50);
	};
	i = 0;
	sprintf(ch, "You got rank %i on the scoreboard!", updateScoreboard(a)+1);
	c = *ch;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {9, 24});
	while(c != '\0') {
		c = *(ch + i++);
		cout << c << flush;
		Sleep(50);
	};
	delete ch;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {18, 26});
	cout << "Play again? (y/n)";
	int KB_code = 0;
	while(true)
		if(kbhit()) {
			KB_code = getch();
			if(KB_code == KB_Y) {
				start();
				break;
			};
			if(KB_code == KB_N || KB_code == KB_ESCAPE) {
				close();
				break;
			}
		}
};

/****** Startup Functions ******/

void setCursor(bool on) {
	CONSOLE_CURSOR_INFO info;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
	info.bVisible = on;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
};

//get time when the game starts
void startClock() {
	starttime = clock();
};

//initialized the random number generator
void initRand() {
	clock_gettime(CLOCK_REALTIME, &mytmhere);
	gen.seed(mytmhere.tv_nsec);
};

//initialize the player
void initCharacter() {
	character->type = PLAYER;
	character->curr = character_origin;
	character->prev = character_origin;
	entities[character_origin.X][character_origin.Y] = PLAYER;
};

//initialized the scoreboard
void initScoreboard() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), C_DEFAULT);
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {sizeX+8, 3});
	cout << "Scoreboard";
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {sizeX+5, 4});
	cout << "----------------";
	for(short i = 1; i <= scores; i++) {
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {sizeX+5, 4+i});
		cout << i << ".";
	};
};

//empties the scoreboard
void clearScoreboard() {
	for(short i = 0; i < scores; i++) {
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {sizeX+7, 5+i});
		cout << "          ";
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), C_DEFAULT);
};

//renders the scoreboard
void renderScoreboard() {
	clearScoreboard();
	for(short i = 0; i < scores; i++) {
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {sizeX+9+(highscores[(int)i]<0?-1:0), 5+i});
		cout << highscores[(int)i];
	}
};

//updates the high scores
int updateScoreboard(int score) {
	int i = 0;
	for(i = 0; i < scores; i++)
		if(score > highscores[i]) {
			for(int j = scores-1; j > i; j--)
				highscores[j] = highscores[j-1];
			highscores[i] = score;
			break;
		}
	renderScoreboard();
	return i;
};

//runs when the program is started for the first time
void onFirst() {
	if(first) {
		setCursor(false);
		initScoreboard();
		initRand();
		first = false;
	};
};

//runs when the program closes
void close() {
	delete companion;
	for(int i = 0; i < total_strangers; i++)
		delete strangers[i];
	delete character;
	setCursor(true);
};

//generates the outer barrier
void generateOuterWall() {
	for (short i = 0; i < (short)sizeX; i++)
		for(short j = 0; j < (short)sizeY; j++)
			if(i == 0 || i == (short)sizeX - 1 || j == 0 || j == (short)sizeY - 1)
				scene[(int)i][(int)j] = WALL;
			else
				scene[(int)i][(int)j] = PATH;
};

//removes all entites from the game
void clearEntities() {
	for(short i = 0; i < (short)sizeX; i++)
		for(short j = 0; j < (short)sizeY; j++)
			entities[(int)i][(int)j] = NONE;
};

//randomly generates strangers and a companion
void generateEntities(int nenemies) {
	clearEntities();
	initCharacter();
	COORD a;
	for(int i = 0; i < nenemies; i++) {
		a.X = distX(gen)*2+1;
		a.Y = distY(gen);
		if(entities[a.X][a.Y] != NONE) {
			i--;
			continue;
		};

		entity* b = new entity;
		b->type = STRANGER;
		b->curr = a;
		b->prev = a;
		strangers[i] = b;
		entities[a.X][a.Y] = STRANGER;
	};
	do {
		a.X = distX(gen)*2+1;
		a.Y = distY(gen);
	} while(entities[a.X][a.Y] != NONE);
	companion->type = COMPANION;
	companion->curr = a;
	companion->prev = a;
	entities[a.X][a.Y] = COMPANION;
};

//does the inital rendering of entities
void renderEntities() {
	//player
	renderEntity(character);
	//strangers
	for(int i = 0; i < total_strangers; i++) {
		renderEntity(strangers[i]);
	};
	//companion
	renderEntity(companion);
};

//handles all initial generation
void generate() {
	generateOuterWall();
	generateEntities(total_strangers);
};

//clears the screen
void clearRender() {
	for(short i = 0; i < (short)sizeX; i++)
		for(short j = 0; j < (short)sizeY+5; j++) {
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {i, j});
			cout << " ";
		}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), C_DEFAULT);
};

//renders walls (to be built on in the future)
void renderScene() {
	clearRender();
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

//a testing function
void testRender() {
	for(short i = 0; i < (short)sizeX; i++)
		for(short j = 0; j < (short)sizeY; j++)
			if(i % 2 == 0 && j % 2 == 0) {
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {i, j});
				cout << M_UNMADE;
			}
};

//performs all game startup tasks
void start() {
	won = false;
	generate();
	renderScene();
	onFirst();
	initCharacter();
	//testRender();
	renderEntities();
	startClock();
	run();
};

//main, calls start to start
int main() {
	system("cls");
	start();
	//read2();

	return 0;
};

