#ifndef GAMEENGINE_H_INCLUDED
#define GAMEENGINE_H_INCLUDED

#include "helperFunctions.h"
#include <tuple>

// Structure of the tuple: evalution, position of next best move
using ratingAndMove = std::tuple<int, int>;

const int gameSpeed = 30;
const int TimeAILine = 6;
const int gameEndLine = TimeAILine + 4;

//char constants 
struct piece
{
	char x;
	char o;
	char empty;
	char point;
	piece()
	{
		x = 'X';
		o = 'O';
		empty = ' ';
		point = '.';
	}
};

enum color
{
	blue = 9,
	green,
	cyan,
	red,
	pink,
	yellow,
	white
};

auto const default_color = white;
auto const win_color = yellow;

//int pair with name
struct coord
{
	int x;
	int y;
	
	coord()
		:x(0),
		 y(0)
	{}

	coord(int _x, int _y)
	{
		x = _x;
		y = _y;
	}
};

class GameEngine
{
public:
	GameEngine();
	void play(); //starts game

private:
	void gameLoop(); 
	void userInput(); 
	void init(); 
	
	void drawBoard(); //draws empty board
	void drawCursor(); //highlights selected square
	void redrawPiece(); //deselects square
	bool const lastMoveWins(int last_move); //checks if last move created 3 in a line
	void gameEnd(); //prints message
	void playerPlays();
	void colorLine(int last_move); //color the winning line
	void colorElements(int frst, int scnd, int thrd);

	//AI methods
	char const getBoardState(); //returns X or O when victory, ' ' when draw, . else
	ratingAndMove maxAlphaBeta(int alpha, int beta);
	ratingAndMove minAlphaBeta(int alpha, int beta);
	void printTimeAI(int recommended, long timeX, long timeO);
	void cleanTimeAI();
	void AIPlays();

private:
	piece pieces; //char consts
	coord toScreen[9]; //const coordinates of playable spaces 
	char board[9]; //pieces played
	int position; //position of player cursor (0-8)
	
	//gameLoop conditions:
	bool xWins;
	bool oWins;
	int movesPlayed;
};

#endif

// 0 1 2
// 3 4 5
// 6 7 8