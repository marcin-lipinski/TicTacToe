#pragma once
#include "Frame.h"
class Game {
private:
	bool gameGoesOn;
	bool playerMove;
	char AISign, playerSign;
	char board[3][3];
	int minimaxDepth;
	Frame *frame;
public:
	Game(int);
	void start();
private:
	bool isTie();
	bool endingGame(char);
	void initVariables();
	void initGameBoard();
	int performPlayerMove();
	void performAIMove();
	void gameLoop();
	void clean();
	char checkWinner();
	char* bestAIMove();
	int minimax(int, bool);
	int initScore(char);
};

