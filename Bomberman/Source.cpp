#include <iostream>
#include <ctime>
#include <Windows.h>
#include <conio.h>
#include <string>			//to remove cout red underline))

using namespace std;

//								KEYS
//	MOVES: ARROWS
//	PLANT BOMB: ENTER
//	DETONATE BOMB: SPACE
//	USE A SWORD: 1
//	USE A PISTOL: 2

//								OBJECTS
//	ENEMY: #
//	BOMB: ¤
//	MEDKIT: 
//	ENERGYDRINK: 
//	COIN: ·
//	BULLET: °

//CONSTANTS

enum obj { HALL, WALL, GOLD, ENEMY, BOMB, MEDKIT, ENERGYDRINK };
enum colors { BLUE = 9, RED = 12, YELLOW = 14, BROWN = 4, PINK = 13, GRAY = 8, PLANTED_BOMB = 9, GREEN = 10, WHITE = 15 };
enum keys { LEFT = 75, UP = 72, RIGHT = 77, DOWN = 80, SPACE = 32, ENTER = 13, ARROW = 224, ONE = 49, TWO = 50 };
enum char_codes { WALL_CODE = 178, GOLD_CODE = 250, ENEMY_CODE = 35, BOMB_CODE = 253, PLAYER_CODE = 1, MEDKIT_CODE = 2, ENERGYDRINK_CODE = 3, BULLET = 248 };

const int h = 30;
const int w = 70;

const int BOMB_DELAY = 15;
const int SWORD_DELAY = 50;
const int BULLET_DELAY = 20;

const int START_HEALTH = 100;
const int START_ENERGY = 500;
const int START_BOMBS = 1;
const int START_COINS = 0;

const int ENERGY_DELAY = 10;			//IN MOVES

const int MEDKIT_POWER = 5;
const int ENERGYDRINK_POWER = 25;

const int BOMB_RADIUS = 3;

const COORD START_COORD = { 0, 1 };
const COORD FINISH_COORD = { w - 1, h - 2 };
const COORD INFO_COORD = { 0, h + 1 };


void print_info(COORD info, HANDLE handle, int coins, int bombs, int health, int energy) {
	SetConsoleCursorPosition(handle, info);
	cout << "                                                                     ";
	SetConsoleCursorPosition(handle, info);
	SetConsoleTextAttribute(handle, GREEN);
	cout << "HEALTH: " << health;
	info.Y++;
	SetConsoleCursorPosition(handle, info);
	cout << "                                                                     ";
	SetConsoleCursorPosition(handle, info);
	SetConsoleTextAttribute(handle, YELLOW);
	cout << "ENEGRY: " << energy;
	info.Y--;
	info.X += 20;
	SetConsoleCursorPosition(handle, info);
	SetConsoleTextAttribute(handle, YELLOW);
	cout << "COINS: " << coins;
	info.Y++;
	SetConsoleCursorPosition(handle, info);
	SetConsoleTextAttribute(handle, BROWN);
	cout << "BOMBS: " << bombs;
}
void win(string s) {
	s = "YOU WON! " + s;
	MessageBoxA(0, s.c_str(), "GAME OVER", MB_OK);
	system("cls");
	//system("taskkill -im cmd.exe /f");
	exit(0);
}
void lose(string s) {
	s = "YOU LOST!:(( " + s;
	MessageBoxA(0, s.c_str(), "GAME OVER", MB_OK);
	system("cls");
	//system("taskkill -im cmd.exe /f");
	exit(0);
}

int main() {
	system("title bomberman");
	system(("mode con cols=" + to_string(w + 10) + " lines=" + to_string(h + 5)).c_str());

	//ARRAY

	int maze[h][w] = {};

	//KEY CODE

	int key;
	int last_key;

	//COORDs

	COORD start = START_COORD;
	COORD p = start;
	COORD finish = FINISH_COORD;
	COORD info = INFO_COORD;
	COORD bomb_coord;
	COORD temp;

	//PLAYERs ITEMS AND COINS AND HEALTH AND ENERGY 
	int coins = START_COINS;
	int bombs = START_BOMBS;
	int health = START_HEALTH;
	int energy = START_ENERGY;

	//FLAGs

	bool is_bomb_planted = false;
	bool is_lose = false;
	bool is_medkit_takeable = false;
	bool is_energydrink_takeable = false;
	int energy_delay = 0;			//in moves
	bool ricochet = false;

	//AMOUNT OF OBJECTS

	int am_coins = 0;
	int am_enemy = 0;

	//RANDOM

	srand(time(0));
	rand();

	//HANDLE

	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

	//HIDE CURSOR

	CONSOLE_CURSOR_INFO ccinfo;
	ccinfo.bVisible = false;
	ccinfo.dwSize = 100;
	SetConsoleCursorInfo(handle, &ccinfo);

	//START FILLING

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			int r = rand() % 6 + 1;
			maze[i][j] = r;

			if (i == 0 || j == 0 || i == h - 1 || j == w - 1)
				maze[i][j] = WALL;

			if (i == start.Y && j == start.X || i == start.Y && j == start.X + 1 || i == finish.Y && j == finish.X || i == finish.Y && j == finish.X - 1 ||
				(pow(i - start.Y, 2) + pow(j - start.X, 2) <= 3 * (pow(BOMB_RADIUS, 2)) && i != 0 && j != 0)) {
				maze[i][j] = HALL;
			}

			if (maze[i][j] == WALL) {
				SetConsoleTextAttribute(handle, GRAY);
				cout << char(WALL_CODE);
			}
			else if (maze[i][j] == GOLD) {
				SetConsoleTextAttribute(handle, YELLOW);
				cout << char(GOLD_CODE);
				am_coins++;
			}
			else if (maze[i][j] == ENEMY) {
				int r = rand() % 60;
				if (r == 0) {
					SetConsoleTextAttribute(handle, RED);
					cout << char(ENEMY_CODE);
					am_enemy++;
				}
				else {
					maze[i][j] = HALL;
					cout << " ";
				}
			}
			else if (maze[i][j] == BOMB) {
				int r = rand() % 100;
				if (r == 0) {
					SetConsoleTextAttribute(handle, BROWN);
					cout << char(BOMB_CODE);
				}
				else {
					maze[i][j] = HALL;
					cout << " ";
				}
			}
			else if (maze[i][j] == MEDKIT) {
				int r = rand() % 50;
				if (r == 0) {
					SetConsoleTextAttribute(handle, GREEN);
					cout << char(MEDKIT_CODE);
				}
				else {
					maze[i][j] = HALL;
					cout << " ";
				}
			}
			else if (maze[i][j] == ENERGYDRINK) {
				int r = rand() % 50;
				if (r == 0) {
					SetConsoleTextAttribute(handle, YELLOW);
					cout << char(ENERGYDRINK_CODE);
				}
				else {
					maze[i][j] = HALL;
					cout << " ";
				}
			}
			else
				cout << " ";

		}
		cout << endl;
	}

	//PUT PLAYER ON START

	SetConsoleCursorPosition(handle, p);
	SetConsoleTextAttribute(handle, BLUE);
	cout << char(PLAYER_CODE);

	print_info(info, handle, coins, bombs, health, energy);

	//GAME

	while (true)
	{
		key = _getch();
		if (key == ARROW) {
			key = _getch();
			last_key = key;
		}

		if (health < 100)
			is_medkit_takeable = true;
		else
			is_medkit_takeable = false;

		if (energy < 500)
			is_energydrink_takeable = true;
		else
			is_energydrink_takeable = false;

		if (maze[p.Y][p.X] == MEDKIT && !is_medkit_takeable) {
			SetConsoleCursorPosition(handle, p);
			SetConsoleTextAttribute(handle, GREEN);
			cout << char(MEDKIT_CODE);
		}
		else if (maze[p.Y][p.X] == ENERGYDRINK && !is_energydrink_takeable) {
			SetConsoleCursorPosition(handle, p);
			SetConsoleTextAttribute(handle, YELLOW);
			cout << char(ENERGYDRINK_CODE);
		}
		else {
			SetConsoleCursorPosition(handle, p);
			cout << " ";
		}

		if (is_bomb_planted) {
			SetConsoleCursorPosition(handle, bomb_coord);
			SetConsoleTextAttribute(handle, PLANTED_BOMB);
			cout << char(BOMB_CODE);
		}

		//MOVEMENT

		if (key == RIGHT && maze[p.Y][p.X + 1] != WALL && p.X + 1 <= w - 1) {
			p.X++;
			energy--;
			energy_delay--;
		}
		else if (key == DOWN && maze[p.Y + 1][p.X] != WALL && p.Y + 1 <= h - 1) {
			p.Y++;
			energy--;
			energy_delay--;
		}
		else if (key == LEFT && maze[p.Y][p.X - 1] != WALL && p.X - 1 >= 0) {
			p.X--;
			energy--;
			energy_delay--;
		}
		else if (key == UP && maze[p.Y - 1][p.X] != WALL && p.Y - 1 >= 0) {
			p.Y--;
			energy--;
			energy_delay--;
		}
		else if (key == ONE && energy > 10) {
			energy -= 10;
			for (int i = p.Y - 1; i <= p.Y + 1; i++)
			{
				for (int j = p.X - 1; j <= p.X + 1; j++)
				{
					if (maze[i][j] == ENEMY) {
						temp.X = j;
						temp.Y = i;
						maze[i][j] = HALL;
						am_enemy--;

						SetConsoleTextAttribute(handle, RED);

						SetConsoleCursorPosition(handle, temp);
						cout << "X";
						Sleep(SWORD_DELAY);
						SetConsoleCursorPosition(handle, temp);
						cout << " ";
					}
				}
			}
		}
		else if (key == TWO && energy > 20) {
			energy -= 20;
			print_info(info, handle, coins, bombs, health, energy);
			temp = p;
			while (true)
			{

				SetConsoleCursorPosition(handle, temp);
				cout << " ";
				Sleep(BULLET_DELAY);
				SetConsoleCursorPosition(handle, p);
				SetConsoleTextAttribute(handle, BLUE);
				cout << char(PLAYER_CODE);

			repeat:

				switch (last_key) {
				case RIGHT:
					temp.X++;
					break;
				case DOWN:
					temp.Y++;
					break;
				case LEFT:
					temp.X--;
					break;
				case UP:
					temp.Y--;
					break;
				}

				SetConsoleTextAttribute(handle, WHITE);
				SetConsoleCursorPosition(handle, temp);
				cout << char(BULLET);
				Sleep(BULLET_DELAY);

				if (temp.X == p.X && temp.Y == p.Y) {
					lose("YOU SHOT YOURSELF!");
					break;
				}
				else if (maze[temp.Y][temp.X] == WALL) {
					if (ricochet == false) {
						switch (last_key) {
						case RIGHT:
							last_key = LEFT;
							break;
						case DOWN:
							last_key = UP;
							break;
						case LEFT:
							last_key = RIGHT;
							break;
						case UP:
							last_key = DOWN;
							break;
						}
						ricochet = true;
						SetConsoleCursorPosition(handle, temp);
						SetConsoleTextAttribute(handle, GRAY);
						cout << char(WALL_CODE);
						goto repeat;
					}
					else {
						ricochet = false;
						SetConsoleCursorPosition(handle, temp);
						SetConsoleTextAttribute(handle, GRAY);
						cout << char(WALL_CODE);
						break;
					}
				}
				else if (maze[temp.Y][temp.X] == ENEMY) {
					maze[temp.Y][temp.X] = HALL;
					am_enemy--;
					SetConsoleCursorPosition(handle, temp);
					SetConsoleTextAttribute(handle, RED);
					cout << "X";
					Sleep(SWORD_DELAY);
					SetConsoleCursorPosition(handle, temp);
					cout << " ";
					ricochet = false;
					break;
				}
				else if (maze[temp.Y][temp.X] == GOLD) {
					SetConsoleCursorPosition(handle, temp);
					SetConsoleTextAttribute(handle, YELLOW);
					cout << char(GOLD_CODE);
					goto repeat;
				}
				else if (maze[temp.Y][temp.X] == MEDKIT) {
					SetConsoleCursorPosition(handle, temp);
					SetConsoleTextAttribute(handle, GREEN);
					cout << char(MEDKIT_CODE);
					goto repeat;
				}
				else if (maze[temp.Y][temp.X] == ENERGYDRINK) {
					SetConsoleCursorPosition(handle, temp);
					SetConsoleTextAttribute(handle, YELLOW);
					cout << char(ENERGYDRINK_CODE);
					goto repeat;
				}
				else if (temp.X == start.X && temp.Y == start.Y || temp.X == finish.X && temp.Y == finish.Y) {
					SetConsoleCursorPosition(handle, temp);
					cout << " ";
					ricochet = false;
					break;
				}

			}
		}

		//BOMB KEYS

		if (key == ENTER && !is_bomb_planted && bombs > 0 && energy > 50) {
			is_bomb_planted = true;
			bomb_coord.X = p.X;
			bomb_coord.Y = p.Y;
			energy -= 49;
			bombs--;
			print_info(info, handle, coins, bombs, health, energy);
		}
		if (key == SPACE && is_bomb_planted) {
			is_bomb_planted = false;
			energy--;
			for (int t = 0; t < 10; t++)
			{
				for (int i = bomb_coord.Y - BOMB_RADIUS; i < bomb_coord.Y + BOMB_RADIUS; i++)
				{
					if (i < 1)
						continue;
					if (i > h - 2)
						break;
					for (int j = bomb_coord.X - BOMB_RADIUS; j < bomb_coord.X + BOMB_RADIUS; j++)
					{
						if (j == p.X && i == p.Y) {
							is_lose = true;
						}

						if (j < 1)
							continue;
						if (j > w - 2)
							break;
						temp.X = j;
						temp.Y = i;
						SetConsoleCursorPosition(handle, temp);

						if (pow(bomb_coord.Y - i, 2) + pow(bomb_coord.X - j, 2) + 1 < pow(BOMB_RADIUS, 2)) {
							int r = rand() % 3;

							if (r == 0)
								SetConsoleTextAttribute(handle, RED);
							else if (r == 1)
								SetConsoleTextAttribute(handle, YELLOW);
							else
								SetConsoleTextAttribute(handle, BROWN);
							cout << "X";
						}
					}
				}
				Sleep(BOMB_DELAY);
			}

			if (is_lose)
				lose("YOU DETONATED YOURSELF!");

			for (int i = bomb_coord.Y - BOMB_RADIUS; i < bomb_coord.Y + BOMB_RADIUS; i++)
			{
				if (i < 1)
					continue;
				if (i > h - 2)
					break;
				for (int j = bomb_coord.X - BOMB_RADIUS; j < bomb_coord.X + BOMB_RADIUS; j++)
				{
					if (j < 1)
						continue;
					if (j > w - 2)
						break;
					temp.X = j;
					temp.Y = i;
					SetConsoleCursorPosition(handle, temp);

					if (pow(bomb_coord.Y - i, 2) + pow(bomb_coord.X - j, 2) + 1 < pow(BOMB_RADIUS, 2)) {
						cout << " ";
						maze[i][j] = HALL;
					}
				}
			}
		}

		SetConsoleCursorPosition(handle, p);
		SetConsoleTextAttribute(handle, BLUE);
		cout << char(PLAYER_CODE);

		if (maze[p.Y][p.X] == GOLD) {
			coins++;
			maze[p.Y][p.X] = HALL;
		}
		if (maze[p.Y][p.X] == BOMB) {
			bombs++;
			maze[p.Y][p.X] = HALL;
		}
		if (maze[p.Y][p.X] == ENEMY) {
			am_enemy--;
			health -= (20 + rand() % 6);
			maze[p.Y][p.X] = HALL;
		}
		if (maze[p.Y][p.X] == MEDKIT && is_medkit_takeable) {
			health += MEDKIT_POWER;
			if (health > START_HEALTH)
				health = START_HEALTH;
			energy_delay = ENERGY_DELAY;
			maze[p.Y][p.X] = HALL;
		}
		if (maze[p.Y][p.X] == ENERGYDRINK && energy_delay <= 0 && is_energydrink_takeable) {
			energy += ENERGYDRINK_POWER;
			if (energy > START_ENERGY)
				energy = START_ENERGY;
			maze[p.Y][p.X] = HALL;
		}

		//LOSE

		if (health <= 0) {
			lose("OUT OF HEALTH!");
			break;
		}
		if (energy <= 0) {
			lose("OUT OF ENERGY!");
			break;
		}

		//WIN

		if (p.X == finish.X && p.Y == finish.Y) {
			win("YOU GOT TO THE FINISH LINE!");
			break;
		}
		if (coins == am_coins) {
			win("YOU COLLECTED ALL COINS!");
			break;
		}
		if (am_enemy == 0) {
			win("YOU KILLED ALL ENEMIES!");
			break;
		}

		print_info(info, handle, coins, bombs, health, energy);
	}
}

