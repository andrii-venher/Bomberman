#include <iostream>
#include <ctime>
#include <Windows.h>
#include <conio.h>
#include <string>			

using namespace std;

//								KEYS
//	MOVES: ARROWS
//	PLANT BOMB: ENTER
//	DETONATE BOMB: SPACE
//	USE A SWORD: 1
//	USE A PISTOL: 2

enum obj { HALL, WALL, GOLD, ENEMY, BOMB, MEDKIT, ENERGYDRINK };
enum colors { BLUE = 9, RED = 12, YELLOW = 14, BROWN = 4, PINK = 13, GRAY = 8, PLANTED_BOMB = 9, GREEN = 10, WHITE = 15 };
enum keys { LEFT = 75, UP = 72, RIGHT = 77, DOWN = 80, SPACE = 32, ENTER = 13, ARROW = 224, ONE = 49, TWO = 50 };
enum char_codes { WALL_CODE = 178, GOLD_CODE = 250, ENEMY_CODE = 2, BOMB_CODE = 253, PLAYER_CODE = 2, MEDKIT_CODE = 3, ENERGYDRINK_CODE = 4, BULLET = 248 };

const int h = 30;
const int w = 70;

int maze[h][w] = {};

HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

const int BOMB_DELAY = 15;
const int SWORD_DELAY = 50;
const int BULLET_DELAY = 10;
const int GETCH_DELAY = 50;

const int START_HEALTH = 100;
const int START_ENERGY = 500;
const int START_BOMBS = 1;
const int START_COINS = 0;

const int ENERGY_DELAY = 10;			

const int MEDKIT_POWER = 5;
const int ENERGYDRINK_POWER = 25;

const int BOMB_RADIUS = 3;

const COORD START_COORD = { 0, 1 };
const COORD FINISH_COORD = { w - 1, h - 2 };
const COORD INFO_COORD = { 0, h + 1 };
