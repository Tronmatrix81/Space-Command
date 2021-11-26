//#include <stdio.h>
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <stdlib.h>
#include <list>
#include <fstream>
#include <string>
using namespace std;

void gotoxy(int x, int y) {
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD dwPos;
	dwPos.X = x;
	dwPos.Y = y;

	SetConsoleCursorPosition(hCon, dwPos);
}

void HideCursor() {
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 2;
	cci.bVisible = FALSE;

	SetConsoleCursorInfo(hCon, &cci);
}

void files(string file, int mode, string seeout = "Hi") {
	if (mode == 1) {
		fstream myfile;
		myfile.open(file, ios::in);
		if (myfile.is_open()) {
			string line;
			int y = 9;
			while (getline(myfile, line)) {
				gotoxy(32, y); cout << line << "\n";
				y += 2;
			}
			myfile.close();
		}
		else {
			printf("A problem occurred while loading data!");
		}
	}
	else if (mode == 2) {
		fstream myfile(file, ios::out);
		if (myfile.is_open()) {
			myfile << seeout << "\n";
			myfile.close();
		}
		else {
			printf("A problem occurred while saving data!");
		}
	}
	else if (mode == 3) {
		fstream myfile(file, ios::app);
		if (myfile.is_open()) {
			myfile << seeout << "\n";
			myfile.close();
		}
		else {
			printf("A problem occurred while saving data!");
		}
	}
}

void Paint_Limits() {
	for (int i = 2; i < 78; i++) {
		gotoxy(i, 3); printf("%c", 205);
		gotoxy(i, 33); printf("%c", 205);
	}

	for (int i = 4; i < 33; i++) {
		gotoxy(2, i); printf("%c", 186);
		gotoxy(77, i); printf("%c", 186);
	}
	
	gotoxy(2, 3); printf("%c", 201);
	gotoxy(2, 33); printf("%c", 200);
	gotoxy(77, 3); printf("%c", 187);
	gotoxy(77, 33); printf("%c", 188);
}

void Screen_Clean() {
	for (int i = 2; i < 33; i++) {
		for (int a = 3; a < 77; a++) {
			gotoxy(a, i); printf(" ");
		}
		if (i == 2) i = 4;
	}
}

class Ship {
	int x, y, hearts, lives = 3;
public:
	Ship(int _x, int _y, int _hearts, int _lives) : x(_x), y(_y), hearts(_hearts), lives(_lives) {};
	int X() { return x; };
	int Y() { return y; }
	void Hearts() { hearts--; }
	void Paint();
	void Erase();
	void Move();
	void Paint_Hearts();
	int Death();
};

void Ship::Paint() {
	gotoxy(x, y); printf("  %c", 30);
	gotoxy(x, y + 1); printf(" %c%c%c", 40, 2, 41);
	gotoxy(x, y + 2); printf("%c%c %c%c", 30, 188, 200, 30);
}

void Ship::Erase() {
	gotoxy(x, y); printf("       ");
	gotoxy(x, y + 1); printf("       ");
	gotoxy(x, y + 2); printf("       ");
}

void Ship::Move() {
	char key;
	if (_kbhit()) {
		key = _getch();
		Erase();
		if (key == 75 && x > 3) x--;;
		if (key == 77 && x + 7 < 77) x++;
		if (key == 72 && y > 4) y--;
		if (key == 80 && y + 3 < 33) y++;
		Paint();
		Paint_Hearts();
	}
}

void Ship::Paint_Hearts() {
	gotoxy(50, 2); printf("Lives: %d", lives);
	gotoxy(62, 2); printf("Health:");
	gotoxy(70, 2); printf("   ");
	for (int i = 0; i < hearts; i++) {
		gotoxy(70 + i, 2); printf("%c", 3);
	}
}

int Ship::Death() {
	if (hearts == 0) {
		lives--;
		if (lives == 0) {
			Paint_Hearts();
			Sleep(1060);
		}
		Erase();
		gotoxy(x, y); printf(" ** ");
		gotoxy(x, y + 1); printf(" *** *");
		gotoxy(x, y + 2); printf(" ** ");
		Sleep(200);
			
		Erase();
		gotoxy(x, y); printf("* *** *");
		gotoxy(x, y + 1); printf("* ** **");
		gotoxy(x, y + 2); printf("* *** *");
		Sleep(200);

		if (lives == 0)	return 0;
		Erase();
		hearts = 3;
		Paint_Hearts();
		Paint();
	}
}

class AST {
	int x, y;
public:
	AST(int _x, int _y) : x(_x), y(_y) {}
	int X() { return x; }
	int Y() { return y; }
	void Paint();
	void Move();
	void Hit(Ship &S);
};

void AST::Paint() {
	gotoxy(x, y); printf("%c", 184); //Change asteroid img //Maybe create gamemode, then random change img
}

void AST::Move() {
	gotoxy(x, y); printf(" ");
	y++;
	if (y > 32) {
		x = rand() % 74 + 3;
		y = 4;
	}
	Paint();
}

void AST::Hit(Ship &S) {
	if (x >= S.X() && x <= S.X() + 6 && y >= S.Y() && y <= S.Y() + 2) {
		S.Hearts();
		S.Erase();
		S.Paint();
		S.Paint_Hearts();
		x = rand() % 74 + 3;
		y = 4;
	}
}

class Bullet {
	int x, y;
public:
	Bullet(int _x, int _y) : x(_x), y(_y) {}
	int X() { return x; }
	int Y() { return y; }
	void Move();
	bool Out();
};

void Bullet::Move() {
	gotoxy(x, y); printf(" ");
	y--;
	gotoxy(x, y); printf("%c", 50); //Change bullet img
}

bool Bullet::Out() {
	if (y == 4) return true;
	return false;
}

class Menu {
	int x, y;
public:
	Menu(int _x, int _y) : x(_x), y(_y) {}
	void Erase();
	void Cursor();
	int Move(bool UpDown = true, int x1=12, int x2=54, int y1=5, int y2=11);
	int Print_End(int time, int points);
};

void Menu::Erase() {
	gotoxy(x, y);
	printf("  ");
}

void Menu::Cursor() {
	gotoxy(x, y);
	printf("//");
	Sleep(250);
	Erase();
	Sleep(250);
}

int Menu::Move(bool UpDown, int x1, int x2, int y1, int y2) {
	char key = 58;
	if (_kbhit()) {
		key = _getch();
		Erase();
		if (UpDown) {
			if (key == 72 && y > y1) {
				y -= 2;
				if (y1 == 5) {
					if (y == y2 - 1) y--;
				}
			}
			if (key == 80 && y < y2) {
				y += 2;
				if (y == y2) y++;
			}
			if (key == 13 or key==77 and y==9 and x==10) return y;
		}
		else {
			if (key == 75) x = x1;
			if (key == 77) x = x2;
			if (key == 13) return x;
		}
	}
	Cursor();
}

int Menu::Print_End(int time, int points) {
	Screen_Clean();
	gotoxy(35, 15); printf("Game Over%c", 33);
	gotoxy(35, 18); printf("Time: %d", time);
	gotoxy(34, 20); printf("Points: %d", points);
	gotoxy(15, 27); printf("Menu");
	gotoxy(57, 27); printf("Exit");
	x = 12; y = 27;
	int ret;
	while (true) {
		ret = Move(false);
		if (ret == 12) return 1;
		if (ret == 54) return 0;
	}
}

int Stats() {
	Screen_Clean();
	gotoxy(40, 5); printf("Stats");
	gotoxy(10, 30); printf("Back");
	gotoxy(19, 30); printf("Exit");
	gotoxy(23, 17); files("PlayerStats.txt", 1);
	Menu ST(7, 30);
	int ret;
	while (true) {
		ret = ST.Move(false, 7, 16);
		if (ret == 7) return 1;
		if (ret == 16) return 0;
	}
}

int Options() {
	gotoxy(27, 7); printf("Controls");
	gotoxy(27, 9); printf("Credits");
	gotoxy(27, 11); printf("Give Feedback");
	Menu OP(24, 7);
	int ret;
	while (true) {
		ret = OP.Move(true, 12, 54, 7, 10);
	}
}

int Start() {
	Screen_Clean();
	gotoxy(13, 5); printf("Play");
	gotoxy(13, 7); printf("Stats");
	gotoxy(13, 9); printf("Options");
	gotoxy(13, 12); printf("Exit");
	gotoxy(4, 32); printf("Beta 0.1");
	Menu S(10, 5);
	int ret, cs = 0;
	while (true) {
		ret = S.Move();
		if (ret == 5) break;
		if (ret == 7) { cs = 1; break; }
		if (ret == 9) { cs = 2; break; }
		if (ret == 12) return 0;
	}
	switch (cs) {
		case 1:
			if (Stats() == 1) Start();
			else return 0;
			break;
		case 2:
			if (Options() == 1) Start();
			else return 0;
			break;
		default:
			return 1;
			break;
	}
}

int main(void) {
	HideCursor();
	Paint_Limits();
	gotoxy(50, 20);
	Menu M(10, 5);
	if (Start() == 0) return 0;
	Screen_Clean();
	Ship S(40, 30, 3, 3);
	S.Paint_Hearts();
	S.Paint();
	list <AST*> A;
	list <AST*>::iterator itA;
	for (int i = 0; i < 7; i++) {
		A.push_back(new AST(rand() % 74 + 3, rand() % 32 + 4));
	}
	list <Bullet*> B;
	list <Bullet*>::iterator itB;
	int points = 0;
	int time = 0;
	bool Game_Over = false;

	while (!Game_Over) {
		gotoxy(4, 2); printf("Points: %d", points);
		gotoxy(20, 2); printf("Time: %d", time);

		if (_kbhit() && _getch() == 32) {
			B.push_back(new Bullet(S.X() + 2, S.Y() - 1));
		}

		for (itB = B.begin(); itB != B.end();) {
			(*itB)->Move();
			if ((*itB)->Out()) {
				gotoxy((*itB)->X(), (*itB)->Y()); printf(" ");
				delete(*itB);
				itB = B.erase(itB);
			}
			else {
				itB++;
			}
		}

		for (itA = begin(A); itA != end(A); itA++) {
			(*itA)->Move();
			(*itA)->Hit(S);
		}

		for (itA = begin(A); itA != end(A);) {
			for (itB = begin(B); itB != end(B);) {
				if ((*itA)->X() == (*itB)->X() && ((*itA)->Y() + 1 == (*itB)->Y() || (*itA)->Y() == (*itB)->Y())) {
					gotoxy((*itB)->X(), (*itB)->Y()); printf(" ");
					delete(*itB);
					itB = B.erase(itB);

					gotoxy((*itA)->X(), (*itA)->Y()); printf(" ");
					delete(*itA);
					itA = A.erase(itA);
					A.push_back(new AST(rand() % 74 + 3, 4));


					points += 100;
				}
				else itB++;
			}
			try { itA++; throw (1); }
			catch (...) { continue; }
		}
		if (S.Death() == 0) Game_Over = true;
		S.Move();
		time += 1;
		Sleep(30);
	}
	int ret = M.Print_End(time, points);
	if (ret == 1) Start();
	if (ret == 0) return 0;
}