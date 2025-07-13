#include <graphics.h>
#include <conio.h>
#include <iostream>
#include <stdlib.h>
#include <ctype.h>

using namespace std;

#define CELL 40
#define ROWS 12
#define COLS 16
#define SCREEN_HEIGHT (CELL * ROWS)
#define SCREEN_WIDTH (CELL * COLS)

enum Tile { EMPTY = 0, WALL = 1, COIN = 2 };

int map[ROWS][COLS] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,2,2,2,1,2,2,2,2,2,2,1,2,2,2,1},
    {1,2,1,2,1,2,1,1,1,1,2,1,2,1,2,1},
    {1,2,1,2,2,2,2,2,2,1,2,2,2,1,2,1},
    {1,2,1,1,1,1,1,1,2,1,1,1,2,1,2,1},
    {1,2,2,2,2,1,2,2,2,2,2,1,2,2,2,1},
    {1,1,1,1,2,1,1,1,1,1,2,1,1,1,1,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,2,1,1,1,1,1,1,1,1,1,1,1,1,2,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};

int pacX = 1, pacY = 1;
int ghostX = 14, ghostY = 1;
char direction = 'd';
int score = 0;
int totalCoins = 0;
int currentPage = 0;
int ghostMoveCounter = 0;

void drawMap() {
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            int px = x * CELL;
            int py = y * CELL;

            if (map[y][x] == WALL) {
                setfillstyle(SOLID_FILL, BLUE);
                bar(px, py, px + CELL, py + CELL);
            } else if (map[y][x] == COIN) {
                setcolor(YELLOW);
                setfillstyle(SOLID_FILL, YELLOW);
                fillellipse(px + CELL / 2, py + CELL / 2, 5, 5);
            }
        }
    }
}

void drawPacman() {
    int cx = pacX * CELL + CELL / 2;
    int cy = pacY * CELL + CELL / 2;

    setfillstyle(SOLID_FILL, YELLOW);
    int start = 0, end = 360;
    switch (direction) {
        case 'w': start = 60; end = 300; break;
        case 's': start = 240; end = 120; break;
        case 'a': start = 150; end = 390; break;
        case 'd': start = 30; end = 330; break;
    }
    pieslice(cx, cy, start, end, CELL / 2 - 2);
}

void drawGhost() {
    int cx = ghostX * CELL + CELL / 2;
    int cy = ghostY * CELL + CELL / 2;
    setfillstyle(SOLID_FILL, RED);
    fillellipse(cx, cy, CELL / 2 - 4, CELL / 2 - 4);
}

bool isWall(int x, int y) {
    return map[y][x] == WALL;
}

void moveGhost() {
    int dx = 0, dy = 0;

    // Try horizontal first
    if (pacX > ghostX && !isWall(ghostX + 1, ghostY)) {
        dx = 1;
    } else if (pacX < ghostX && !isWall(ghostX - 1, ghostY)) {
        dx = -1;
    }
    else if (pacY > ghostY && !isWall(ghostX, ghostY + 1)) {
        dy = 1;
    } else if (pacY < ghostY && !isWall(ghostX, ghostY - 1)) {
        dy = -1;
    }

    ghostX += dx;
    ghostY += dy;
}

void gameOver() {
    settextstyle(3, 0, 4);
    setcolor(RED);
    outtextxy(150, 200, (char*)"GAME OVER!");
    getch();
    closegraph();
    system("pause"); // Keeps console open on Windows
    exit(0);
}

void winGame() {
    settextstyle(3, 0, 4);
    setcolor(GREEN);
    outtextxy(180, 200, (char*)"YOU WIN!");
    getch();
    closegraph();
    system("pause"); // Keeps console open on Windows
    exit(0);
}

void displayScore() {
    settextstyle(0, 0, 1);
    setcolor(WHITE);
    char buf[50];
    sprintf(buf, "Score: %d / %d", score, totalCoins);
    outtextxy(10, SCREEN_HEIGHT - 15, buf);
}

int main() {
    initwindow(SCREEN_WIDTH, SCREEN_HEIGHT + 20, "Pac-Man Final Optimal");

    for (int y = 0; y < ROWS; y++)
        for (int x = 0; x < COLS; x++)
            if (map[y][x] == COIN) totalCoins++;

    while (true) {
        setactivepage(currentPage);
        setvisualpage(1 - currentPage);
        clearviewport();

        drawMap();
        drawPacman();
        drawGhost();
        displayScore();

        if (pacX == ghostX && pacY == ghostY)
            gameOver();
        if (score == totalCoins)
            winGame();

        delay(60);  // Smooth, not too fast

        ghostMoveCounter++;
        if (ghostMoveCounter >= 2) {  // Moves every 2nd frame
            moveGhost();
            ghostMoveCounter = 0;
        }

        if (kbhit()) {
            char ch = tolower(getch());
            direction = ch;

            int newX = pacX;
            int newY = pacY;

            if (ch == 'w') newY--;
            else if (ch == 's') newY++;
            else if (ch == 'a') newX--;
            else if (ch == 'd') newX++;

            if (!isWall(newX, newY)) {
                pacX = newX;
                pacY = newY;

                if (map[pacY][pacX] == COIN) {
                    score++;
                    map[pacY][pacX] = EMPTY;
                }
            }
        }

        currentPage = 1 - currentPage;
    }

    closegraph();
    return 0;
}
