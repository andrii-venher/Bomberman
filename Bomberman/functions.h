#include "CONSTANTS.h"

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
	exit(0);
}
void lose(string s) {
	s = "YOU LOST!:(( " + s;
	MessageBoxA(0, s.c_str(), "GAME OVER", MB_OK);
	system("cls");
	exit(0);
}

void is_win(COORD p, int coins, int am_coins, int am_enemy) {
	if (p.X == FINISH_COORD.X && p.Y == FINISH_COORD.Y) {
		win("YOU GOT TO THE FINISH LINE!");
	}
	if (coins == am_coins) {
		win("YOU COLLECTED ALL COINS!");
	}
	if (am_enemy == 0) {
		win("YOU KILLED ALL ENEMIES!");
	}
}
void is_lose(int health, int energy) {
	if (health <= 0) {
		lose("OUT OF HEALTH!");
	}
	if (energy <= 0) {
		lose("OUT OF ENERGY!");
	}
}

void init() {
	system("title bomberman");
	system(("mode con cols=" + to_string(w + 10) + " lines=" + to_string(h + 5)).c_str());

	srand(time(0));
	rand();

	CONSOLE_CURSOR_INFO ccinfo;
	ccinfo.bVisible = false;
	ccinfo.dwSize = 100;
	SetConsoleCursorInfo(handle, &ccinfo);
}

void start_filling(int& am_coins, int& am_enemy) {
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			int r = rand() % 6 + 1;
			maze[i][j] = r;

			if (i == 0 || j == 0 || i == h - 1 || j == w - 1)
				maze[i][j] = WALL;

			if (i == START_COORD.Y && j == START_COORD.X || i == START_COORD.Y && j == START_COORD.X + 1 || i == FINISH_COORD.Y && j == FINISH_COORD.X || i == FINISH_COORD.Y && j == FINISH_COORD.X - 1 ||
				(pow(i - START_COORD.Y, 2) + pow(j - START_COORD.X, 2) <= 3 * (pow(BOMB_RADIUS, 2)) && i != 0 && j != 0)) {
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
}

void obj_print(COORD c, colors color, char code) {
	SetConsoleCursorPosition(handle, c);
	SetConsoleTextAttribute(handle, color);
	cout << code;
}
void obj_print(COORD c, char ch) {
	SetConsoleCursorPosition(handle, c);
	cout << ch;
}

bool is_medkit_takeable(int health) {
	if (health < START_HEALTH)
		return true;
	else
		return false;
}
bool is_energydrink_takeable(int energy, int energy_delay) {
	if (energy < START_ENERGY && energy_delay <= 0)
		return true;
	else
		return false;
}
void is_takeable(COORD p, int health, int energy, int energy_delay) {
	if (maze[p.Y][p.X] == MEDKIT && !(is_medkit_takeable(health))) {
		obj_print(p, GREEN, MEDKIT_CODE);
	}
	else if (maze[p.Y][p.X] == ENERGYDRINK && !(is_energydrink_takeable(energy, energy_delay))) {
		obj_print(p, YELLOW, ENERGYDRINK_CODE);
	}
	else {
		obj_print(p, ' ');
	}
}

void position(COORD p, int& health, int& energy, int& energy_delay, int& coins, int& bombs, int& am_enemy) {
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
	if (maze[p.Y][p.X] == MEDKIT && is_medkit_takeable(health)) {
		health += MEDKIT_POWER;
		if (health > START_HEALTH)
			health = START_HEALTH;
		energy_delay = ENERGY_DELAY;
		maze[p.Y][p.X] = HALL;
	}
	if (maze[p.Y][p.X] == ENERGYDRINK && energy_delay <= 0 && is_energydrink_takeable(energy, energy_delay)) {
		energy += ENERGYDRINK_POWER;
		if (energy > START_ENERGY)
			energy = START_ENERGY;
		maze[p.Y][p.X] = HALL;
	}
}

void get_key(int& key, int& last_key, bool is_shoot) {
	if (_kbhit()) {
		key = _getch();
		if (key == ARROW) {
			key = _getch();
			if (is_shoot)
				return;
			last_key = key;
		}
	}
	else
	{
		key = NULL;
		Sleep(GETCH_DELAY);
	}
		
}

void actions(int key,int& last_key, COORD& p, COORD& bomb_coord, COORD& bullet_coord, int& health, int& energy, int& energy_delay,
	int& bombs, bool& is_bomb_planted, int& am_enemy, bool& is_shoot, bool& ricochet) {
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
					COORD temp;
					temp.X = j;
					temp.Y = i;
					maze[i][j] = HALL;
					am_enemy--;

					obj_print(temp, RED, 'X');
					Sleep(SWORD_DELAY);
					obj_print(temp, ' ');
				}
			}
		}
	}
	else if (key == TWO && energy > 20) {
		energy -= 20;
		is_shoot = true;
		bullet_coord = p;
	}
	else if (key == ENTER && !is_bomb_planted && bombs > 0 && energy > 50) {
		is_bomb_planted = true;
		bomb_coord.X = p.X;
		bomb_coord.Y = p.Y;
		energy -= 49;
		bombs--;
	}
	else if (key == SPACE && is_bomb_planted) {
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
						health = 0;
					}

					if (j < 1)
						continue;
					if (j > w - 2)
						break;
					COORD temp;
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

		is_lose(health, energy);

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
				COORD temp;
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
	if (is_shoot) {
		obj_print(bullet_coord, ' ');
		Sleep(BULLET_DELAY);
		obj_print(p, BLUE, PLAYER_CODE);

	repeat:

		switch (last_key) {
		case RIGHT:
			bullet_coord.X++;
			break;
		case DOWN:
			bullet_coord.Y++;
			break;
		case LEFT:
			bullet_coord.X--;
			break;
		case UP:
			bullet_coord.Y--;
			break;
		}

		obj_print(bullet_coord, WHITE, BULLET);
		Sleep(BULLET_DELAY);

		if (bullet_coord.X == p.X && bullet_coord.Y == p.Y) {
			health = 0;
			return;
		}
		else if (maze[bullet_coord.Y][bullet_coord.X] == WALL) {
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
				obj_print(bullet_coord, GRAY, WALL_CODE);
				goto repeat;
			}
			else {
				obj_print(bullet_coord, GRAY, WALL_CODE);
				is_shoot = false;
				ricochet = false;
				return;
			}
		}
		else if (maze[bullet_coord.Y][bullet_coord.X] == ENEMY) {
			maze[bullet_coord.Y][bullet_coord.X] = HALL;
			am_enemy--;
			obj_print(bullet_coord, RED, 'X');
			Sleep(SWORD_DELAY);
			obj_print(bullet_coord, ' ');
			is_shoot = false;
			ricochet = false;
			return;
		}
		else if (maze[bullet_coord.Y][bullet_coord.X] == GOLD) {
			Sleep(GETCH_DELAY);
			obj_print(bullet_coord, YELLOW, GOLD_CODE);
			goto repeat;
		}
		else if (maze[bullet_coord.Y][bullet_coord.X] == MEDKIT) {
			Sleep(GETCH_DELAY);
			obj_print(bullet_coord, GREEN, MEDKIT_CODE);
			goto repeat;
		}
		else if (maze[bullet_coord.Y][bullet_coord.X] == ENERGYDRINK) {
			Sleep(GETCH_DELAY);
			obj_print(bullet_coord, YELLOW, ENERGYDRINK_CODE);
			goto repeat;
		}
		else if (bullet_coord.X == START_COORD.X && bullet_coord.Y == START_COORD.Y || bullet_coord.X == FINISH_COORD.X && bullet_coord.Y == FINISH_COORD.Y) {
			obj_print(bullet_coord, ' ');
			is_shoot = false;
			ricochet = false;
			return;
		}
	}
}

void game() {
	//KEY CODE

	int key;
	int last_key;

	//COORDs

	COORD p = START_COORD;
	COORD bomb_coord;
	COORD bullet_coord;

	//PLAYERs ATTR
	int coins = START_COINS;
	int bombs = START_BOMBS;
	int health = START_HEALTH;
	int energy = START_ENERGY;

	//FLAGs

	bool is_bomb_planted = false;
	int energy_delay = 0;			
	bool is_shoot = false;
	bool ricochet = false;

	//AMOUNT OF OBJECTS

	int am_coins = 0;
	int am_enemy = 0;

	//START FILLING

	start_filling(am_coins, am_enemy);

	//PUT PLAYER ON START

	obj_print(p, BLUE, PLAYER_CODE);

	//GAME

	while (true)
	{
		get_key(key, last_key, is_shoot);

		is_takeable(p, health, energy, energy_delay);

		if (is_bomb_planted) {
			obj_print(bomb_coord, PLANTED_BOMB, BOMB_CODE);
		}

		actions(key, last_key, p, bomb_coord, bullet_coord, health, energy, energy_delay,
			bombs, is_bomb_planted, am_enemy, is_shoot, ricochet);

		print_info(INFO_COORD, handle, coins, bombs, health, energy);

		obj_print(p, BLUE, PLAYER_CODE);

		position(p, health, energy, energy_delay, coins, bombs, am_enemy);

		is_lose(health, energy);

		is_win(p, coins, am_coins, am_enemy);

		print_info(INFO_COORD, handle, coins, bombs, health, energy);
	}
}
