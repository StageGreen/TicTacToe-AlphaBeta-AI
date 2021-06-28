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
	gotoxy(0, gameEndLine);
	if (xWins)
		cout << "You won!" << endl;
	else if (oWins)
		cout << "You lost!" << endl;
	else
		cout << "Draw." << endl;
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
			playerPlays();
			// If board is full the last move of O shouldnt be played
			if (movesPlayed < 9) {
				AIPlays();
				drawCursor();
			}
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

bool const GameEngine::lastMoveWins(int last_move)
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

void GameEngine::playerPlays()
{
	gotoxy(toScreen[position].y, toScreen[position].x);
	cout << pieces.x;
	board[position] = pieces.x;
	movesPlayed++;
	if (lastMoveWins(position))
		xWins = true;
}

char const GameEngine::getBoardState()
{
	//Check rows
	for (size_t i = 0; i < 3; i++)
	{
		int row = i * 3;
		if (board[row] != pieces.empty &&
			board[row] == board[row + 1] &&
			board[row + 1] == board[row + 2])
			return board[row];
	}

	//Check columns
	for (size_t i = 0; i < 3; i++)
	{
		if (board[i] != pieces.empty &&
			board[i] == board[i + 3] &&
			board[i + 3] == board[i + 2 * 3])
			return board[i];
	}

	//Check diagonals
	if (board[2] != pieces.empty &&
		board[2] == board[4] && board[4] == board[6])
		return board[2];

	if (board[0] != pieces.empty &&
		board[0] == board[4] && board[4] == board[8])
		return board[0];

	//Check for full board
	for (size_t i = 0; i < 9; i++)
	{
		if (board[i] == pieces.empty)
			break;
		if (i == 8)
			return pieces.empty;
	}

	//Game continues
	return pieces.point;
}

void GameEngine::printTimeAI(int recommended, long timeX, long timeO)
{
	cleanTimeAI();
	gotoxy(0, TimeAILine);
	cout << "Elapsed time for O move: " << timeO << " microseconds ";
	gotoxy(0, TimeAILine + 1);
	cout << "Recommended move for X: " << recommended % 3 << ", " << recommended / 3;
	gotoxy(0, TimeAILine + 2);
	cout << "Elapsed time for recommendation of X move: " << timeX << " microseconds ";
}

void GameEngine::cleanTimeAI()
{
	const int numberOfLines = 3;
	const int widthOfText = 60;
	for (size_t i = 0; i < numberOfLines; i++)
	{
		gotoxy(0, TimeAILine + i);
		for (size_t i = 0; i < widthOfText; i++)
		{
			cout << ' ';
		}
	}
}

// Player "O" is max, in this case AI
ratingAndMove GameEngine::maxAlphaBeta(int alpha, int beta)
{
	//Possible values:
	// 1 - win
	// 0 - tie
	// -1 - lose
	int maxValue = -2;

	// Best move hasnt been calculated
	int move = -1;

	//Set the base cases - win, loss, tie
	char boardState = getBoardState();
	if (boardState == pieces.x) {
		return ratingAndMove(-1, 0);
	}
	if (boardState == pieces.o) {
		return ratingAndMove(1, 0);
	}
	if (boardState == pieces.empty) {
		return ratingAndMove(0, 0);
	}

	for (int i = 0; i < 9; i++) {
		if (board[i] == pieces.empty) {
			// On a empty square we start evaluating the
			// possible move(and branch of moves) player "O" can make
			board[i] = pieces.o;
			ratingAndMove nextBestMove = minAlphaBeta(alpha, beta);
			// Compare the evaluation of the moves and set if better
			if (get<0>(nextBestMove) > maxValue) {
				maxValue = get<0>(nextBestMove);
				move = i;
			}
			board[i] = pieces.empty;
			// The alpha beta prune happens in these 2 ifs
			if (maxValue >= beta)
				return ratingAndMove(maxValue, move);
			if (maxValue > alpha)
				alpha = maxValue;
		}
	}
	return ratingAndMove(maxValue, move);
}

// Player "X" is min, in this case human
ratingAndMove GameEngine::minAlphaBeta(int alpha, int beta)
{
	//Possible values:
	// 1 - lost
	// 0 - tie
	// -1 - win
	int minValue = 2;

	int move = -1;

	char boardState = getBoardState();
	if (boardState == pieces.x) {
		return ratingAndMove(-1, 0);
	}
	if (boardState == pieces.o) {
		return ratingAndMove(1, 0);
	}
	if (boardState == pieces.empty) {
		return ratingAndMove(0, 0);
	}

	for (int i = 0; i < 9; i++) {
		if (board[i] == pieces.empty) {
			board[i] = pieces.x;
			ratingAndMove nextBestMove = maxAlphaBeta(alpha, beta);
			if (get<0>(nextBestMove) < minValue) {
				minValue = get<0>(nextBestMove);
				move = i;
			}
			board[i] = pieces.empty;
			if (minValue <= alpha)
				return ratingAndMove(minValue, move);
			if (minValue < beta)
				beta = minValue;
		}
	}
	return ratingAndMove(minValue, move);
}

void GameEngine::AIPlays()
{
	// Timers for compariong min/max with alpha beta pruning optimization
	std::chrono::steady_clock::time_point beginO = std::chrono::steady_clock::now();
	ratingAndMove bestOMove = maxAlphaBeta(-2, 2);
	std::chrono::steady_clock::time_point endO = std::chrono::steady_clock::now();
	long timeO = (long)std::chrono::duration_cast<std::chrono::microseconds>(endO - beginO).count();
	position = get<1>(bestOMove);
	gotoxy(toScreen[position].y, toScreen[position].x);
	cout << pieces.o;
	board[position] = pieces.o;
	movesPlayed++;
	if (lastMoveWins(position))
		oWins = true;
	// Evaluate X best move and recommend it
	std::chrono::steady_clock::time_point beginX = std::chrono::steady_clock::now();
	ratingAndMove bestXMove = minAlphaBeta(-2, 2);
	std::chrono::steady_clock::time_point endX = std::chrono::steady_clock::now();
	long timeX = (long)std::chrono::duration_cast<std::chrono::microseconds>(endX - beginX).count();
	printTimeAI(get<1>(bestXMove), timeX, timeO);
}
