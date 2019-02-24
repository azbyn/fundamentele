/*
Laborator 1. 15
 */

#include "profanity.h"
#include "rect.h"
#include "point.h"

using namespace azbyn::profanity;
using azbyn::Rect;
using azbyn::Point;

#include <curses.h>
#include <assert.h>
#include <time.h>
#include <math.h>

#include <iostream>
#include <ctime>

struct Pile {
    const int* ptr;
    int len;
    Pile(const int* ptr, int len) : ptr(ptr), len(len) {}
};

struct Graphics {
    static constexpr int pancakeBlockLen = 2;
    enum Pairs {
        PAIR_BG = 1,
        PAIR_REAL,
        PAIR_FAKE,
        PAIR_SCALE,
    };

    void initColors() {
        //std::cout <<"INIT COLS\n";
        start_color();
        auto textCol = COL_BLACK;
        //init_pair(short pair, short f, short b);
        init_pair(PAIR_BG, COL_WHITE, COL_BLACK);
        init_pair(PAIR_REAL, textCol, 226/*220/*COL_DARK_YELLOW*/);
        init_pair(PAIR_FAKE, textCol, 208/*166/*COL_ORANGE*/);
        init_pair(PAIR_SCALE, textCol, COL_DARK_GRAY);
    }
    int realWeight;
    Graphics(int realWeight) : realWeight(realWeight) {
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

    static constexpr int topSpace = 3;
    static constexpr int leftSpace = 5;
    static constexpr int coinWidth = 10;
    static constexpr int scaleHeight = 20;

    void drawPiles(const int* first, Pile left, Pile right, Pile leftover) {
        colfill(PAIR_BG, Rect(0, 0, COLS, LINES));
        assert(left.len == right.len);

        int bottom = scaleHeight + topSpace + 5;
        for (int i = 0; i < left.len; ++i) {
            int y = bottom - i;
            drawCoin(leftSpace, y, first, left.ptr+i);
            drawCoin(leftSpace+coinWidth + 4, y, first, right.ptr+i);
        }

        for (int i = 0; i < leftover.len; ++i) {
            int y = bottom - i;
            drawCoin(leftSpace+ coinWidth *2 + 8, y, first, leftover.ptr+i);
        }
        setcol(PAIR_BG);
    }



    //scaleDelta: negative numbers for left, 0 for center, and positive for right
    void draw(int scaleDelta, const int* first, Pile left, Pile right, Pile leftover) {
        // Thrown together just to work
        colfill(PAIR_BG, Rect(0, 0, COLS, LINES));
        assert(left.len == right.len);

        setcol(PAIR_SCALE);

        int bottom = scaleHeight + topSpace;
        int sideLength = coinWidth * 2 + 8;

        int height = scaleHeight + 6;
        int x = sideLength + 6;
        mvvline(topSpace, x, ' ', height);
        mvvline(topSpace, x + 1, ' ', height);
        int x1 = x / 2 + (x%2) + 6;

        mvhline(topSpace, x-x1/2, ' ', x1);
        mvhline(topSpace+height, x - x1/2, ' ', x1);

        int leftDelta = (scaleDelta < 0 ? -1 : (scaleDelta > 0));
        int leftDelta2 = leftDelta * 2;

        int leftSpaceTop = sideLength / 2;
        leftSpaceTop += leftSpaceTop % 2;
        mvhline(topSpace -leftDelta, 2 + leftSpaceTop / 2+4, ' ', sideLength - leftSpaceTop);
        mvhline(topSpace +leftDelta, 4 + sideLength + 8 + leftSpaceTop/2-4, ' ', sideLength-leftSpaceTop);

        mvhline(bottom + 1-leftDelta2, 2, ' ', sideLength);
        mvhline(bottom + 1+leftDelta2, 4 + sideLength + 8, ' ', sideLength);


        int i = 0;
        for (; i < left.len/2; ++i) {
            int y = bottom - i;
            drawCoin(leftSpace, y-leftDelta2, first, left.ptr+i);
            drawCoin(leftSpace + x + 4, y+leftDelta2, first, right.ptr+i);
        }
        int startI = i;
        for (; i < left.len; ++i) {
            int y = bottom - i + startI;
            drawCoin(leftSpace+coinWidth + 2, y-leftDelta2, first, left.ptr+i);
            drawCoin(leftSpace + x + 4+coinWidth + 2, y+leftDelta2, first, right.ptr+i);
        }
        
        for (int i = 0; i < leftover.len; ++i) {
            int y = height + topSpace - i;
            drawCoin(sideLength * 2 + 8+ 10, y, first, leftover.ptr+i);
        }
        setcol(PAIR_BG);
    }

    void drawCoin(int x, int y, const int* first, const int* coin) {
        setcol(*coin < realWeight ? PAIR_FAKE : PAIR_REAL);
        mvhline(y, x, ' ', coinWidth);
        //we offset the index by 1 so that it shows the 0-th coin as the first
        mvprintw(y, x + coinWidth / 2 - 1, "%d", 1 + coin - first);
    }
};

void waitForKey() { getch(); }

constexpr int realWeight = 40;
constexpr int fakeWeight = 35;

double log3(double x) {
    return log(x) / log(3);

}

int main() {
    std::srand(std::time(nullptr));
    constexpr int maxCoinsLen = 81;
    int length = maxCoinsLen;
    int coins[maxCoinsLen];

    do {
        std::cout << "Please insert number of coins (< "<< maxCoinsLen <<"): ";
        std::cin >> length;
    } while (length > maxCoinsLen);

    for (int i = 0; i < length; ++i) {
        coins[i] = realWeight;
    }
    coins[std::rand() % length] = fakeWeight;

    Graphics g(realWeight);
    Pile bigPile(coins, length);
    int steps = 0;
    int expectedSteps = ceil(log3(length));

    // we split the coins in 3 piles. the first 2 piles get the same number of coins
    // the difference between the first two piles and the last must be at most 1
    while (bigPile.len > 1) {
        ++steps;
        int pileSize = bigPile.len / 3 + (bigPile.len % 3 == 2);
        // Pile(const int* ptr, int len)
        Pile left(bigPile.ptr, pileSize);
        Pile right(bigPile.ptr+pileSize, pileSize);

        //a negative length is not a problem
        Pile leftover(bigPile.ptr + 2 * pileSize, bigPile.len - 2 *pileSize);
        g.drawPiles(coins, left, right, leftover);
        mvprintw(0, 0, "Currently on step %d, this should take about %d steps", steps, expectedSteps);

        waitForKey();

        int difference = 0;

        for (int i = 0; i < left.len; ++i)
            difference += right.ptr[i] - left.ptr[i];

        g.draw(difference, coins, left, right, leftover);
        mvprintw(0, 0, "Currently on step %d, this should take about %d steps", steps, expectedSteps);
        waitForKey();

        if (difference == 0)
            bigPile = leftover;
        else if (difference > 0)
            bigPile = left;
        else bigPile = right;
    }
    assert(bigPile.len == 1);

    mvprintw(0, 0, "Took %d steps (expected %d) to find that coin #%d is fake", steps, expectedSteps, bigPile.ptr - coins+1);
    mvprintw(1, 0, "Press q to quit.");

    while (tolower(getch()) != 'q') {}

    return 0;
}
