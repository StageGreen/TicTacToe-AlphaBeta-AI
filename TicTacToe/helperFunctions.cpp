#include <iostream>
#include <windows.h>

#include "helperFunctions.h"

void hideCursor()
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(handle, &cursorInfo);
	cursorInfo.bVisible = false; // set the cursor visibility
	SetConsoleCursorInfo(handle, &cursorInfo);
}

void gotoxy(short x, short y)
{
	if (x < 0 || y < 0)
		throw std::invalid_argument("negative console coordinates!");
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD position = { x, y };
	SetConsoleCursorPosition(handle, position);
}

void setColor(short color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}