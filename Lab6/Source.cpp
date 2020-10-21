#include<stdio.h>
#include <windows.h>
#include<conio.h>
#include<time.h>
#include<stdlib.h>

char cursor(int x, int y) {
	HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
	char buf[2]; COORD c = { x,y }; DWORD num_read;
	if (
		!ReadConsoleOutputCharacter(hStd, (LPTSTR)buf, 1, c, (LPDWORD)&num_read))

		return '\0';
	else
		return buf[0];
}
void gotoxy(int x, int y)
{
	COORD v = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), v);
}
void setcolor(int fg, int bg)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, bg * 16 + fg);
}
void erase_ship()
{
	setcolor(0, 0);
	printf("       ");
}
void draw_ship(int x, int y)
{
	gotoxy(x - 1, y);
	erase_ship();
	gotoxy(x + 1, y);
	erase_ship();
	gotoxy(x, y);
	setcolor(7, 9);
	printf("(|-0-|)");
}
void draw_bullet(int bx, int by)
{
	gotoxy(bx, by);
	setcolor(6, 0);
	printf("A");
}
void erase_bullet(int bx, int by)
{
	gotoxy(bx, by);
	setcolor(0, 0);
	printf(" ");
}
void setcursor(bool visible)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = 20;
	SetConsoleCursorInfo(console, &lpCursor);
}
void draw_enemy(int x, int y) 
{
	setcolor( rand() % 10, 0);
	gotoxy(x, y);
	printf("*");
}
void displayscore(int x, int y, int score)
{
	setcolor(7, 8);
	gotoxy(x, y);
	printf("Score = %d" , score);
}

int main()
{
	char ch = ' ';
	int x = 40, y = 25;
	int stateship = 0;
	int statebul = 0;
	int bx[5], by[5], tbn[5] = { 0 };
	int i;
	int countEx[20];
	int countEy[20];
	int Ex, Ey;
	int enemy;
	int score = 0;

		for (enemy = 0; enemy < 20; enemy++) {
			Ey = rand() % 5 + 2;
			Ex = rand() % 80 + 5;
			draw_enemy(Ex, Ey);
			//save position enemy to Ex and Ey
			countEx[enemy] = Ex;
			countEy[enemy] = Ey;
		}
	draw_ship(x, y);
	do {
		setcursor(0);
		if (_kbhit()) {
			ch = _getch();
			if (ch == 'a') { stateship = 1; }
			if (ch == 'd') { stateship = 2; }
			if (ch == 's') { stateship = 0; }
			if (ch == ' ') { statebul = 1; }
			fflush(stdin);
		}
		if (stateship == 1 && x > 0) { x--; draw_ship(x, y); }
		if (stateship == 2 && x < 80) { x++; draw_ship(x, y); }
		if (stateship == 0) { draw_ship(x, y); }
		if (statebul == 1)
		{
			for (int i = 0; i < 5; i++)
			{
				if (tbn[i] == 0)
				{
					statebul = 1;  bx[i] = x + 3; by[i] = y - 1;
					tbn[i] = 1;
					Beep(500, 60);
					break;
				}
			}
			statebul = 0;
		}
		for (int i = 0; i < 5; i++)
		{
			if (tbn[i] == 1) {
				int isCollide = 0;
				for (int e = 0; e < 20; e++) {
					if (bx[i] == countEx[e] && by[i] == countEy[e]) { //If position bullet == position enemy is collide = ture
						isCollide = true;
						score++;
						Beep(300, 60);
						Ey = rand() % 5 + 2;
						Ex = rand() % 80 + 5;
						draw_enemy(Ex, Ey);
					}
				}
				erase_bullet(bx[i], by[i]);
				if (by[i] == 0 || isCollide) //delete bullet when y=0 or collide = ture
				{ 
					tbn[i] = 0; 
				}
				else { draw_bullet(bx[i], --by[i]); }
				
			}
			displayscore(90, 0 ,score);
		}
		Sleep(100);
	} while (ch != 'x');
	return 0;
}