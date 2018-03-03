// 迷宫.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#define HEIGHT 60
#define WIDTH 150
#define RATE 40
#include <iostream>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include <gdiplus.h>
#include <process.h>
#pragma comment(lib, "Gdiplus.lib")
class person
{
private:
	int x=1, y=1;
	bool Pass=0;
public:
	void move();
	bool end();
};
void gotoxy(int ,int);
//

//
int endx = WIDTH - 2;
int endy = HEIGHT - 2;
int map[WIDTH+100][HEIGHT+100] = { 0 };
bool index[WIDTH][HEIGHT] = { 0 };
short int vec[4][2] =
{
	1,0,
	0,1,
	0,-1,
	-1,0
};
bool DFS(int x, int y)
{
	index[x][y] = 1;
	if (x == endx&&y == endy)
		return true;
	for (int i = 0; i < 4; i++)
	{
		if (index[x + vec[i][0]][y + vec[i][1]] == 0 && map[x + vec[i][0]][y + vec[i][1]] != 1)
			if (DFS(x + vec[i][0], y + vec[i][1]))
			{
				map[x + vec[i][0]][y + vec[i][1]] = 10;
				return true;
			}
	}
	return false;
}
int ey = 0, ex = 0;
void draw(void *arg);
void draw2(void *arg);
using namespace std;


int main()
{
	//GDI
	//cursor setting
	CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
	ShowCursor(0);
	srand(time(0));
r:
	printf("\r生成中 . . . . ");
	for (int i = 0; i < HEIGHT; i++)
		map[0][i] = map[WIDTH-1][i] = 1;
	for (int i = 0; i < WIDTH; i++)
		map[i][0] = map[i][HEIGHT - 1] = 1;
	for (int x = 1; x < WIDTH - 1; x++)
		for (int y = 1; y < HEIGHT - 1; y++)
		{
			int n = rand() % 100 ;
			map[x][y] = (n <= RATE);
		}
	map[1][1] = 0;
	map[WIDTH - 2][HEIGHT - 2] = 3;
	if (!DFS(1, 1))
	{
		memset(map, 0, sizeof(map));
		memset(index, 0, sizeof(index));
		goto r;
	}
	system("cls");
	map[WIDTH - 2][HEIGHT - 2] = 3;
	person a;
	gotoxy(0, 0);
	while (1)
	{
		_beginthread(draw, 0, 0);
		_beginthread(draw2, 0, 0);
		a.move();
		if (a.end())
			break;
		Sleep(50);
	}
    return 0;
}

void person::move()
{
	char key = getch();
	map[x][y] = 0;
	switch (key)
	{
	case 'w':y--; if (map[x][y]==1)y++; break;
	case 'a':x--; if (map[x][y]==1)x++; break;
	case 'd':x++; if (map[x][y]==1)x--; break;
	case 's':y++; if (map[x][y]==1)y--; break;

	}
	map[x][y] = 2;
	if (x > 20&&ex<WIDTH-12)
		ex = x - 20;
	if (y > 12&&ey<HEIGHT-12)
		ey = y - 20;
	if (x <= 20)
		ex = 0;
	if (y <= 20)
		ey = 0;
	if (x == endx&&y == endy)
	{
		cout << "Congratulations!!!";
		Pass = 1;
	}
}

bool person::end()
{
	return Pass;
}

void gotoxy(int x, int y)
{
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void draw(void *arg)
{
	int p = 20;
	HWND hw = GetConsoleWindow();
	HDC hdc = GetDC(hw);
	HPEN pen = CreatePen(0, 1, RGB(0, 0, 130));
		for (int y = ey; y < 25+ey; y++)
		{
			for (int x = ex; x < 40+ex; x+=2)
			{
				HBRUSH wall = CreateSolidBrush(RGB(150, 150, 150));
				HBRUSH blank = CreateSolidBrush(RGB(0, 0, 0));
				HBRUSH man = CreateSolidBrush(RGB(255, 0, 0));
				HBRUSH end = CreateSolidBrush(RGB(0, 255, 255));
				HBRUSH tip = CreateSolidBrush(RGB(0, 50, 50));
				int tempx=x, tempy=y;
				if (map[x][y]==1)
				{
					x -= ex; y -= ey;
					SelectObject(hdc,wall);
					Rectangle(hdc, x *p ,y*p, x*p+p, y * p+p);
				}
				else if(map[x][y]==0)
				{
					x -= ex; y -= ey;
					SelectObject(hdc,blank);
					Rectangle(hdc, x * p, y * p, x * p + p, y * p + p);
				}
				else if (map[x][y] == 3)
				{
					x -= ex; y -= ey;
					SelectObject(hdc, end);
					Rectangle(hdc, x * p, y * p, x * p + p, y * p + p);
				}
				else if (map[x][y] == 10)
				{
					x -= ex; y -= ey;
					SelectObject(hdc, tip);
					Rectangle(hdc, x * p, y * p, x * p + p, y * p + p);
				}
				else 
				{
					x -= ex; y -= ey;
					SelectObject(hdc, man);
					Rectangle(hdc, x * p, y * p, x * p + p, y * p + p);
				}
				DeleteObject(wall);
				DeleteObject(man);
				DeleteObject(blank);
				DeleteObject(end);
				DeleteObject(tip);
				x = tempx; y = tempy;
			}
		}
	ReleaseDC(hw,hdc);
	_endthread();
}
void draw2(void *arg)
{
	int p = 20;
	HWND hw = GetConsoleWindow();
	HDC hdc = GetDC(hw);
	HPEN pen = CreatePen(0, 1, RGB(0, 0, 130));
	for (int y = ey; y < 25 + ey; y++)
	{
		for (int x = 39+ex; x >=ex; x-=2)
		{
			HBRUSH wall = CreateSolidBrush(RGB(150, 150, 150));
			HBRUSH blank = CreateSolidBrush(RGB(0, 0, 0));
			HBRUSH man = CreateSolidBrush(RGB(255, 0, 0));
			HBRUSH end = CreateSolidBrush(RGB(0, 255, 255));
			HBRUSH tip = CreateSolidBrush(RGB(0, 50, 50));
			int tempx = x, tempy = y;
			if (map[x][y] == 1)
			{
				x -= ex; y -= ey;
				SelectObject(hdc, wall);
				Rectangle(hdc, x *p, y*p, x*p + p, y * p + p);
			}
			else if (map[x][y] == 0)
			{
				x -= ex; y -= ey;
				SelectObject(hdc, blank);
				Rectangle(hdc, x * p, y * p, x * p + p, y * p + p);
			}
			else if (map[x][y] == 3)
			{
				x -= ex; y -= ey;
				SelectObject(hdc, end);
				Rectangle(hdc, x * p, y * p, x * p + p, y * p + p);
			}
			else if (map[x][y] == 10)
			{
				x -= ex; y -= ey;
				SelectObject(hdc, tip);
				Rectangle(hdc, x * p, y * p, x * p + p, y * p + p);
			}
			else
			{
				x -= ex; y -= ey;
				SelectObject(hdc, man);
				Rectangle(hdc, x * p, y * p, x * p + p, y * p + p);
			}
			DeleteObject(wall);
			DeleteObject(man);
			DeleteObject(blank);
			DeleteObject(end);
			DeleteObject(tip);
			x = tempx; y = tempy;
		}
	}
	ReleaseDC(hw, hdc);
	_endthread();
}
