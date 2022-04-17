#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include <conio.h>
#include <vector>
using namespace std;
using std::cout;

bool running = true;
int _x = 35, _y = 7;
char frame[21][81];
char cannon_area[3][5];


class bullet {
public:
	int x, y,nx,ny;
};
vector<bullet> bulletList;


void change_cannon_state(int i) {
	for (int y = 0; y < 3; y++)
		for (int x = 0; x < 5; x++)
			cannon_area[y][x] = ' ';
	cannon_area[1][2] = '+';
	switch (i%8)
	{
	case 0:
		cannon_area[0][2] = '|';
		break;
	case 1:
		cannon_area[0][3] = '/';
		break;
	case 2:
		cannon_area[1][3] = '-'; cannon_area[1][4] = '-';
		break;
	case 3:
		cannon_area[2][3] = '\\';
		break;
	case 4:
		cannon_area[2][2] = '|';
		break;
	case 5:
		cannon_area[2][1] = '/';
		break;
	case 6:
		cannon_area[1][0] = '-'; cannon_area[1][1] = '-';
		break;
	case 7:
		cannon_area[0][1] = '\\';
		break;
	default:
		break;
	}
}

bullet new_bullet(int _i) {
	int _nx,_ny;
	bullet b;
	int i = _i % 8;
	if (i == 1 || i == 2 || i == 3) _nx = 1;
	else if (i == 5 || i == 6 || i == 7) _nx = -1;
	else _nx = 0;
	if (i == 1 || i == 0 || i == 7) _ny = -1;
	else if (i == 5 || i == 3 || i == 4) _ny = 1;
	else _ny = 0;
	b.x = _x; b.y = _y;
	switch (i) {
	case 0:
		b.x += 2;
		break;
	case 1:
		b.x += 3;
		break;
	case 2:
		b.x += 4; b.y += 1;
		break;
	case 3:
		b.x += 3; b.y += 2;
		break;
	case 4:
		b.x += 2; b.y += 2;
		break;
	case 5:
		b.x += 1; b.y += 2;
		break;
	case 6:
		b.y += 1;
		break;
	case 7:
		b.x += 1;
		break;
	default:
		break;
	}
	b.nx = _nx;
	b.ny = _ny;

	return b;
}

void clear() {
	for (int y = 0; y < 20; y++)
		for (int x = 0; x < 80; x++)
			frame[y][x] = ' ';

	for (int y = 0; y < 20; y++)
		frame[y][80] = '|';
	for (int y = 0; y < 81; y++)
		frame[20][y] = '=';
	frame[20][80] = '^';
}

int main()
{
	
	HANDLE hStdout;
	COORD destCoord;
	destCoord.X = 0; destCoord.Y = 0;
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	clear();
	
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = false;
	SetConsoleCursorInfo(hStdout, &lpCursor);
	int rotation = 0;
	change_cannon_state(rotation);
	while (running)
	{
		clock_t start = clock();
		clear();

		cout << _y << " "<<_x;
		//input
		if (_kbhit()) {
			switch (_getch())
			{
			case 77:
				rotation++;
				change_cannon_state(rotation);
				break;
			case 75:
				rotation--;
				if (rotation < 0) rotation = 7;
				change_cannon_state(rotation);
				break;
			case 'w':
				if (_y > 0) { for (int x = 0; x < 3; x++)frame[_y + 2][_x + x + 1] = ' '; _y -= 1; }
				break;
			case 'a':
				if (_x > 1) frame[_y + 1][_x + 4] = ' '; _x -= 1;
				break;
			case 's':
				if (_y < 17)for (int x = 0; x < 3; x++)frame[_y][_x + x + 1] = ' '; _y += 1;
				break;
			case 'd':
				if (_x < 75)frame[_y + 1][_x] = ' '; _x += 1;
				break;
			case 32:

				bulletList.push_back(new_bullet(rotation));
				break;
			default:
				break;
			}
		}
		//render cannon
		for (int y = 0; y < 3; y++) {
			for (int x = 0; x < 5; x++) {
				frame[y + _y][x + _x] = cannon_area[y][x];
			}
		}

		//render bullets
		for (int i = 0; i < bulletList.size(); i++) {
			if (!(bulletList[i].x + bulletList[i].nx >= 80 || bulletList[i].x + bulletList[i].nx < 0 || bulletList[i].y + bulletList[i].ny >= 20 || bulletList[i].y + bulletList[i].ny < 0)) {
				bulletList[i].x += bulletList[i].nx; bulletList[i].y += bulletList[i].ny;
				frame[bulletList[i].y][bulletList[i].x] = 'o';
			}
		}
		
		//drawframe
		for (int y = 0; y < 21; y++) {
			string s = "";
			for (int x = 0; x < 81; x++) {
				s += frame[y][x];
			}
			cout << s << "\n" << flush;
		}
		Sleep(30);
		double duration = (clock() - start) / (double)CLOCKS_PER_SEC;
		cout << floor(1/duration) << " fps";
		SetConsoleCursorPosition(hStdout, destCoord);

	}
	return 0;

}

