#include "Game.h"
#include <iostream>

Game::Game(int depth) {
	initVariables();
	minimaxDepth = depth;
}

void Game::initGameBoard() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			board[i][j] = ' ';
		}
	}
}

void Game::initVariables() {
	initGameBoard();
	gameGoesOn = true;
	playerMove = true;
	frame = new Frame(300, 300);
	AISign = 'x';
	playerSign = 'o';
}

char Game::checkWinner() {
	if (board[1][1] != ' ') {
		char sign = board[1][1];
		if (board[0][0] == sign && board[2][2] == sign) return sign;
		if (board[2][0] == sign && board[0][2] == sign) return sign;
		if (board[1][0] == sign && board[1][2] == sign) return sign;
		if (board[0][1] == sign && board[2][1] == sign) return sign;
	}
	if (board[0][0] != ' ') {
		char sign = board[0][0];
		if (board[1][0] == sign && board[2][0] == sign) return sign;
		if (board[0][1] == sign && board[0][2] == sign) return sign;
	}
	if (board[2][2] != ' ') {
		char sign = board[2][2];
		if (board[2][0] == sign && board[2][1] == sign) return sign;
		if (board[0][2] == sign && board[1][2] == sign) return sign;
	}
	return ' ';
}

bool Game::isTie() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (board[i][j] == ' ') return false;
		}
	}
	return true;
}

char* Game::bestAIMove() {
	int actDepth = 0;
	char bestMoves[2];
	int bestScore = -999999;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (board[i][j] == ' ') {
				board[i][j] = AISign;
				int score = minimax(actDepth + 1, false);
				if (score > bestScore) {
					bestScore = score;
					bestMoves[0] = i;
					bestMoves[1] = j;
				}
				board[i][j] = ' ';
			}
		}
	}
	return bestMoves;
}

int Game::minimax(int depth, bool maximazing) {
	int score = initScore(checkWinner());
	if (isTie() || score != 0 || depth > minimaxDepth) return score;

	int bestScore = maximazing ? -999999 : 999999;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (board[i][j] == ' ') {
				board[i][j] = maximazing ? AISign : playerSign;
				score = minimax(depth + 1, !maximazing);
				bestScore = maximazing ? (bestScore < score ? score : bestScore) : (bestScore > score ? score : bestScore);
				board[i][j] = ' ';
			}
		}
	}
	return bestScore;
}

int Game::initScore(char winner) {
	if (winner == AISign) return 1;
	if (winner == playerSign) return -1;
	return 0;
}

void Game::start() {
	gameLoop();
}

void Game::gameLoop() {

	while (gameGoesOn) {
		if (playerMove) {
			if(performPlayerMove() == -1) gameGoesOn = false;
		}
		else {
			performAIMove();
		}

		playerMove = !playerMove;
		if (endingGame(checkWinner())) gameGoesOn = false;
	}
	clean();
}

void Game::performAIMove() {
	char* movesToPerform = bestAIMove();
	board[movesToPerform[0]][movesToPerform[1]] = AISign;
	frame->setBoxTakenAndDraw(movesToPerform[0], movesToPerform[1], AISign);
}

int Game::performPlayerMove()  {
	bool doRepeat = true;
	char* movesToPerform = {};
	while (doRepeat) {
		movesToPerform = frame->handlePlayerChoice(playerSign);
		if (movesToPerform == NULL) return -1;
		if (board[movesToPerform[0]][movesToPerform[1]] == ' ') doRepeat = false;
	}
	board[movesToPerform[0]][movesToPerform[1]] = playerSign;
}

bool Game::endingGame(char sign) {
	if (sign == AISign || sign == playerSign) {
		frame->showBoardAndWait(sign);
		return true;
	}
	if (isTie()) {
		frame->showBoardAndWait(' ');
		return true;
	}
	return false;
}

void Game::clean() {
	frame->clean();
	return;
}
