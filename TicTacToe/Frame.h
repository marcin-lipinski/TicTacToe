#pragma once
#include <SDL2/SDL.h>
class Frame
{
private:
	SDL_Window* win;
	SDL_Renderer* ren;
	int windowWidth, windowHeight;
	int boxWidth, boxHeight;
	int lineWeight;	
	int* activeBox;
	int*** boxesCoordinates;
public:
	Frame(int, int);
	void drawShape(int, int, char);
	void clean();
	void setBoxTakenAndDraw(int, int, char);
	void showBoardAndWait(char);
	char* handlePlayerChoice(char);
private:
	void initVariables(int, int);
	void initBoxes();
	void initWindow();
	void createWindow();
	void printGameboard();
	void drawCircle(int, int);
	void drawCross(int, int);
	void changeBoxInner(int, int, char);
	void resetBox(int, int);
	void clearFrameAndShowBoard(const char*);
	char* getActiveBoxIndexes(char);
};



