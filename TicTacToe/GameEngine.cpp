#include <iostream>
#include <windows.h>
#include <chrono>
#include "GameEngine.h"

using namespace std;

GameEngine::GameEngine()
	:xWins(false)
	,oWins(false)
	,movesPlayed(0)
	,toScreen
	{
		coord(0,1), coord(0,5), coord(0,9),
		coord(2,1), coord(2,5), coord(2,9),
		coord(4,1), coord(4,5), coord(4,9)
	}
{
	//init board with empty char
	memset(board, pieces.empty, 9);
	position = 0;
}

void GameEngine::play()
{
	init();
	gameLoop();
	gameEnd();
}

void GameEngine::gameEnd()
{
	gotoxy(3, toScreen[8].x + 2);
	if (xWins)
		cout << "You won!";
	else if (oWins)
		cout << "You lost!";
	else
		cout << "Draw.";
}

void GameEngine::gameLoop()
{
	while (!xWins && !oWins && movesPlayed < 9)
	{
		auto start_s = std::chrono::steady_clock::now();

		userInput();

		auto stop_s = std::chrono::steady_clock::now();
		auto calcTime = std::chrono::duration <double, std::milli>(stop_s - start_s).count();
		Sleep(max(3500.0 / gameSpeed - calcTime, 0));
	}
}

void GameEngine::userInput()
{
	if (GetAsyncKeyState(VK_UP))
	{
		if (position > 2)
		{
			redrawPiece();
			position -= 3;
			drawCursor();
		}
	}
	if (GetAsyncKeyState(VK_DOWN))
	{
		if (position < 6)
		{
			redrawPiece();
			position += 3;
			drawCursor();
		}
	}
	if (GetAsyncKeyState(VK_LEFT))
	{
		if (position % 3 != 0)
		{
			redrawPiece();
			position -= 1;
			drawCursor();
		}
	}
	if (GetAsyncKeyState(VK_RIGHT))
	{
		if (position % 3 != 2)
		{
			redrawPiece();
			position += 1;
			drawCursor();
		}
	}
	if (GetAsyncKeyState(VK_SPACE))
	{
		if (board[position] == pieces.empty) //if player can play here
		{
			gotoxy(toScreen[position].y, toScreen[position].x);
			cout << pieces.x;
			board[position] = pieces.x;
			movesPlayed++;
			if (isVictory(position))
				xWins = true;
			
			/*
			position = AI( ... )
			gotoxy(toScreen[position].y, toScreen[position].x);
			cout << pieces.o;
			board[position] = pieces.o;
			movesPlayed++;
			if (isVictory(position))
				oWins = true;
			*/
		}
	}
}

void GameEngine::init()
{
	drawBoard();
	drawCursor();
}

void GameEngine::drawBoard()
{
	cout << "   |   |  \n";
	cout << "-----------\n";
	cout << "   |   |  \n";
	cout << "-----------\n";
	cout << "   |   |  \n";
	gotoxy(0, 0);
}

void GameEngine::drawCursor()
{
	gotoxy(toScreen[position].y, toScreen[position].x);
	setColor(green);
	if (board[position] != pieces.empty)
		cout << board[position];
	else
		cout << pieces.point;
	setColor(default_color);
}

void GameEngine::redrawPiece()
{
	gotoxy(toScreen[position].y, toScreen[position].x);
	cout << board[position];
}

bool GameEngine::isVictory(int last_move)
{
	if (last_move % 2 == 0) //if move is even, check diagonals 
	{
		if (last_move != 0 && last_move != 8 &&
			board[2] == board[4] && board[4] == board[6])
			return true;
		if (last_move != 2 && last_move != 6 &&
			board[0] == board[4] && board[4] == board[8])
			return true;
	}

	int row = last_move / 3; //number of row (0-2)
	int first_el = row * 3; //first element of the row of the last moved piece
	
	if (board[first_el] == board[first_el + 1] && 
		board[first_el + 1] == board[first_el + 2])
		return true;

	int column = last_move % 3; //number of column (0-2)
	if (board[column] == board[column + 3] && 
		board[column + 3] == board[column + 2 * 3])
		return true;
	return false; //no victory found
}
