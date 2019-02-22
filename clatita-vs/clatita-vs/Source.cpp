/*
Laborator 1. 16
 */

#include "profanity.h"
#include "rect.h"
#include "point.h"

using namespace azbyn::profanity;
using azbyn::Rect;
using azbyn::Point;

#include "curses.h"

#include <iostream>
#include <algorithm>

constexpr bool explicitMode = true;

struct Graphics {
	static constexpr int pancakeBlockLen = 2;
	enum Pairs {
		PAIR_BG = 1,
		PAIR_PANCAKE,
		PAIR_SPATULA_BLADE,
		PAIR_SPATULA_HANDLE,
		PAIR_PLATE,
	};

	void initColors() {
		//std::cout <<"INIT COLS\n";
		start_color();
		auto textCol = COL_BLACK;
		//init_pair(short pair, short f, short b);
		init_pair(PAIR_BG, COL_WHITE, COL_BLACK);
		init_pair(PAIR_PANCAKE, textCol, /*COL_DARK_YELLOW*/172);
		init_pair(PAIR_SPATULA_BLADE, textCol, COL_LIGHT_GRAY);
		init_pair(PAIR_SPATULA_HANDLE, textCol, 166);
		init_pair(PAIR_PLATE, textCol, COL_WHITE);
	}

	int plateSize;
	Graphics(int biggestPancake) : plateSize(biggestPancake) {
		setlocale(LC_ALL, "");
		initscr(); // initialize the curses library
		keypad(stdscr, true); // enable keyboard mapping
		nonl(); // tell curses not to do NL->CR/NL on output
		cbreak(); // take input chars one at a time, no wait for \n
		noecho();
		//nodelay(stdscr, true);
		meta(stdscr, true);
		curs_set(0);
		
		if (has_colors())
			initColors();
	}
	~Graphics() {
		endwin();
	}

	static constexpr int topSpace = 5;
	static constexpr int leftSpace = 4;

	void draw(const int* pancakeStack, int size, int spatulaIndex) {
		colfill(PAIR_BG, Rect(0, 0, COLS, LINES));

		setcol(PAIR_PANCAKE);
		int i = 0;

		if (spatulaIndex > 0) {
			for (; i < spatulaIndex; ++i) {
				drawBlin(i + topSpace, pancakeStack[i]);
			}
			drawSpatula(spatulaIndex + topSpace);
		}

		setcol(PAIR_PANCAKE);
		for (; i < size; ++i) {
			drawBlin(i + topSpace + 1, pancakeStack[i]);
		}

		drawPlate(size + topSpace + 1);
	}


	void drawSpatula(int y) {
		setcol(PAIR_SPATULA_BLADE);
		int padding = (plateSize)* pancakeBlockLen / 2 - 2;
		int bladeLen = padding + 6;

		mvhline(y, padding + leftSpace, ' ', bladeLen);
		if (explicitMode) {
			constexpr char text[] = "spatula";
			int pad2 = (bladeLen - sizeof(text)) / 2;
			mvprintw(y, pad2 + leftSpace + padding, text);
		}

		mvhline(y - 1, plateSize * pancakeBlockLen + leftSpace + 0, ' ', 4);
		mvhline(y - 2, plateSize * pancakeBlockLen + leftSpace + 2, ' ', 4);
		setcol(PAIR_SPATULA_HANDLE);
		mvhline(y - 3, plateSize * pancakeBlockLen + leftSpace + 4, ' ', 30);
		mvhline(y - 4, plateSize * pancakeBlockLen + leftSpace + 6, ' ', 30);
	}
	void drawBlin(int y, int len) { // Blin == pancake
		drawLine(y, len);
		if (explicitMode) {
			constexpr int maxDigits = 2;
			int padding = (plateSize * pancakeBlockLen - maxDigits) / 2;
			mvprintw(y, leftSpace + padding, "%*d", maxDigits / 2, len);
		}
	}
	void drawPlate(int y) {
		setcol(PAIR_PLATE);

		mvhline(y - 1, 0, ' ', 2);
		mvhline(y - 1, plateSize*pancakeBlockLen + 6, ' ', 2);

		drawLine(y, plateSize + 2);
		if (explicitMode) {
			constexpr char text[] = "farfurie";
			int padding = (plateSize * pancakeBlockLen - sizeof(text)) / 2;
			mvprintw(y, leftSpace + padding, text);
		}
	}
	void drawLine(int y, int len) {
		int padding = (plateSize - len) * pancakeBlockLen / 2;
		mvhline(y, padding + leftSpace, ' ', pancakeBlockLen*len);
	}


};

void waitForKey() { getch(); }

void swap(int& a, int& b) {
	std::swap(a, b);
}

//get the biggest pancake with an index bigger than index
//purely for visual purposes
int getSpatulaIndex(const int* vec, int length) {
	int biggestIndex = 0;
	for (int j = 0; j < length; ++j) {
		if (vec[j] >= vec[biggestIndex]) biggestIndex = j;
	}
	return biggestIndex;
}
void flip(int* vec, int index) {
	for (int i = 0; i < index / 2; ++i) {
		swap(vec[i], vec[index - i - 1]);
	}
}

int main() {
	constexpr int maxStackLen = 20;
	constexpr int maxPancakeWidth = 30;
	constexpr int randomSize = maxStackLen;
	int pancakeStack[maxStackLen];
	int length;
	int biggest = 0;

	std::cout << "Tip: you can enter a negative number to generate " << randomSize << " random-sized pancakes\n";
	std::cout << "Tip: pancake sizes must be between 1 and " << maxPancakeWidth << "\n";
	do {
		std::cout << "Please insert number of pancakes (< 50): ";
		std::cin >> length;
	} while (length > maxStackLen);

	if (length < 0) {
		length = randomSize;
		for (int i = 0; i < length; ++i) {
			int res = std::rand() % 20 + 1;
			if (res > biggest) biggest = res;
			pancakeStack[i] = res;
		}
	}
	else {
		int res;
		for (int i = 0; i < length; ++i) {
			do {
				std::cout << "p[" << i + 1 << "] = ";
				std::cin >> res;
			} while (res <= 0 || res > maxPancakeWidth);
			if (res > biggest) biggest = res;
			pancakeStack[i] = res;
		}
	}
	Graphics g(biggest);
	g.draw(pancakeStack, length, 0);
	int steps = 0;


	mvprintw(0, 0, "Press any key to advance");
	for (int bottom = length; bottom >= 1; --bottom) {
		int si = getSpatulaIndex(pancakeStack, bottom) + 1;

		//if the biggest is at the bottom we do nothing
		if (si == bottom) continue;
		waitForKey();

		//if the biggest is already at the top we don't have to flip it
		if (si != 1) {
			g.draw(pancakeStack, length, si);
			waitForKey();

			flip(pancakeStack, si);
			++steps;
		}
		g.draw(pancakeStack, length, bottom);
		waitForKey();

		flip(pancakeStack, bottom);
		g.draw(pancakeStack, length, 0);
		++steps;
	}
	mvprintw(0, 0, "Done in %d steps. Please press q", steps);
	while (tolower(getch()) != 'q') {}
	return 0;
}
