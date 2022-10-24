#include "Frame.h"
#include <SDL2/SDL.h>
#include <iostream>

Frame::Frame(int width, int height) {
	initVariables(width, height);
	initBoxes();
	initWindow();
	createWindow();
	printGameboard();
}

void Frame::initWindow() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::printf("error initializing SDL: %s\n", SDL_GetError());
	}
}

void Frame::createWindow() {
	win = SDL_CreateWindow("Tic Tac Toe", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_BORDERLESS & SDL_WINDOW_SHOWN);
	ren =  SDL_CreateRenderer(win, -1, 0);
}

void Frame::initVariables(int w, int h){
	lineWeight = 6;
	windowHeight = h + 2 * lineWeight;
	windowWidth = w + 2 * lineWeight;
	boxWidth = (windowWidth - 2 * lineWeight) / 3;
	boxHeight = (windowHeight - 2 * lineWeight) / 3;
	activeBox = (int*)malloc(2 * sizeof(int*));
}

void Frame::printGameboard(){
	SDL_SetRenderDrawColor(ren, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(ren);
	SDL_SetRenderDrawColor(ren, 55, 55, 5, SDL_ALPHA_OPAQUE);
	int offset = (2 * lineWeight) / 4;
	int offset_2 = lineWeight + (2 * lineWeight) / 4;
	
	SDL_RenderDrawLine(ren, boxWidth + offset, 0, boxWidth + offset, windowHeight);
	SDL_RenderDrawLine(ren, 2 * boxWidth + offset_2, 0, 2 * boxWidth + offset_2, windowHeight);
	SDL_RenderDrawLine(ren, 0, boxHeight + offset, windowWidth, boxHeight + offset);
	SDL_RenderDrawLine(ren, 0, 2 * boxHeight + offset_2, windowWidth, 2 * boxHeight + offset_2);

	SDL_RenderPresent(ren);
}

void Frame::initBoxes() {
	boxesCoordinates = (int***)malloc(3 * sizeof(int***));
	int t_x = 0, t_y = 0;

	for (int i = 0; i < 3; i++) {
		boxesCoordinates[i] = (int**)malloc(3 * sizeof(int***));
		for (int j = 0; j < 3; j++) {
			boxesCoordinates[i][j] = (int*)malloc(3 * sizeof(int*));
			boxesCoordinates[i][j][0] = t_x;
			boxesCoordinates[i][j][1] = t_y;
			boxesCoordinates[i][j][2] = 0;
			t_x += boxWidth + lineWeight;
		}
		t_x = 0;
		t_y += boxHeight + lineWeight;
	}
}

void Frame::drawShape(int row, int column, char sign) {
	SDL_SetRenderDrawColor(ren, 128, 128, 128, 100);
	if (sign == 'o') {
		drawCircle(row, column);
	}
	else drawCross(row, column);
	SDL_RenderPresent(ren);
}

void Frame::drawCircle(int i, int j) {
	const int radius = (windowWidth - 2 * lineWeight) / 6;
	const int diameter = 2 * radius;

	int x = (radius - 1);
	int y = 0;
	int tx = 1;
	int ty = 1;
	int error = (tx - diameter);
	int centreX = boxesCoordinates[i][j][0] + radius;
	int centreY = boxesCoordinates[i][j][1] + radius;

	while (x >= y){
		SDL_RenderDrawPoint(ren, centreX + x, centreY - y);
		SDL_RenderDrawPoint(ren, centreX + x, centreY + y);
		SDL_RenderDrawPoint(ren, centreX - x, centreY - y);
		SDL_RenderDrawPoint(ren, centreX - x, centreY + y);
		SDL_RenderDrawPoint(ren, centreX + y, centreY - x);
		SDL_RenderDrawPoint(ren, centreX + y, centreY + x);
		SDL_RenderDrawPoint(ren, centreX - y, centreY - x);
		SDL_RenderDrawPoint(ren, centreX - y, centreY + x);

		if (error <= 0)
		{
			++y;
			error += ty;
			ty += 2;
		}

		if (error > 0)
		{
			--x;
			tx += 2;
			error += (tx - diameter);
		}
	}
	SDL_RenderPresent(ren);
}

void Frame::drawCross(int i, int j) {
	int x1 = boxesCoordinates[i][j][0] + lineWeight;
	int y1 = boxesCoordinates[i][j][1] + lineWeight;
	int x2 = x1 + (windowWidth - 2 * lineWeight) / 3 - 2 * lineWeight;
	int y2 = y1 + (windowWidth - 2 * lineWeight) / 3 - 2 * lineWeight;
	SDL_RenderDrawLine(ren, x1, y1, x2, y2);
	SDL_RenderDrawLine(ren, x1, y2, x2, y1);
}

char* Frame::handlePlayerChoice(char sign) {
	SDL_Event evnt;
	bool doRepeat = true;

	while (doRepeat) {
		SDL_PollEvent(&evnt);
		if (evnt.type == SDL_QUIT) return NULL;
		if (evnt.type == SDL_MOUSEMOTION) {
			changeBoxInner(evnt.button.x, evnt.button.y, sign);
		}
		if (evnt.type == SDL_MOUSEBUTTONDOWN) {
			return getActiveBoxIndexes(sign);
		}
	}	
}

void Frame::changeBoxInner(int x, int y, char sign) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (boxesCoordinates[i][j][2] == 1) {
				resetBox(i, j);
				boxesCoordinates[i][j][2] = 0;
			}
			if (boxesCoordinates[i][j][2] == 0) {
				if (boxesCoordinates[i][j][0] <= x && boxesCoordinates[i][j][0] + boxWidth >= x) {
					if (boxesCoordinates[i][j][1] <= y && boxesCoordinates[i][j][1] + boxWidth >= y) {
						drawShape(i, j, sign);
						boxesCoordinates[i][j][2] = 1;
						activeBox[0] = i; activeBox[1] = j;
					}
				}
			}
		}
	}
}

void Frame::resetBox(int i, int j) {
	SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
	SDL_Rect drawBox;
	drawBox.x = boxesCoordinates[i][j][0];
	drawBox.y = boxesCoordinates[i][j][1];
	drawBox.w = boxWidth;
	drawBox.h = boxHeight;
	SDL_RenderFillRect(ren, &drawBox);
}

void Frame::setBoxTakenAndDraw(int i, int j, char sign) {
	boxesCoordinates[i][j][2] = 2;
	drawShape(i, j, sign);
}

char* Frame::getActiveBoxIndexes(char sign) {
	char indexes[2] = { activeBox[0], activeBox[1] };
	setBoxTakenAndDraw(indexes[0], indexes[1], sign);
	return indexes;
}

void Frame::showBoardAndWait(char sign) {
	if (sign == 'x')clearFrameAndShowBoard("images\\x_win.bmp");
	if (sign == 'o')clearFrameAndShowBoard("images\\o_win.bmp");
	if (sign == ' ')clearFrameAndShowBoard("images\\tie.bmp");

	SDL_Event evnt;
	bool doRepeat = true;
	while (doRepeat) {
		SDL_PollEvent(&evnt);
		if (evnt.type == SDL_QUIT) return;
		if (evnt.type == SDL_MOUSEBUTTONDOWN) return;
	}
}

void Frame::clearFrameAndShowBoard(const char* uri) {
	SDL_Surface *boardBMP = SDL_LoadBMP(uri);
	SDL_Texture* boardTexture = SDL_CreateTextureFromSurface(ren, boardBMP);
	SDL_SetTextureAlphaMod(boardTexture, 122);
	SDL_Rect board;
	board.w = windowWidth;
	board.h = windowHeight;
	board.x = 0;
	board.y = 0;

	SDL_RenderCopy(ren, boardTexture, NULL, &board);
	SDL_RenderPresent(ren);
}

void Frame::clean() {
	SDL_DestroyWindow(win);
	SDL_DestroyRenderer(ren);
	SDL_Quit();
}