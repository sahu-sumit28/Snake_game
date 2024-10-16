#include <ncurses.h>      // ncurses library
#include <cstdlib>        // For rand(), srand(), exit()
#include <ctime>          // For time()
#include <unistd.h>       // For usleep() (POSIX systems only)
#include <cstdio>         // For C-style file I/O (fopen, fscanf, fprintf, fclose)
#include <iostream>       // For standard I/O streams (cin, cout)
#include <string>         // For string operations


using namespace std;

int width, height;
int headX, headY, fruitX, fruitY, score;
bool gameOver;
int tailX[100], tailY[100];
int tailLength;
int highestScore = 0;

FILE *s;
 
const char *file = "highscore.txt";

void getHigh()
{
    s = fopen(file, "r");
    fscanf(s, "%d", &highestScore);
    fclose(s);
}
void writeHigh()
{
    s = fopen(file, "w");
    fprintf(s, "%d", highestScore);
    fclose(s);
}

void gotoXY(int x, int y)
{
    move(y, x);
}

enum directions
{
    stop = 0,
    leftss,
    rightss,
    up,
    down
};
directions d;

void setup()
{
    gameOver = false;
    d = stop;
    headX = width / 2;
    headY = height / 2;
    srand(time(0));

    do {
        fruitX = rand() % (width - 1) + 1;
        fruitY = rand() % (height - 1) + 1;
    } while (fruitX == headX && fruitY == headY);
}

void draw()
{
    clear();
    start_color();              // Initialize colors
    init_pair(1, COLOR_RED, COLOR_BLACK); // Snake color
    init_pair(2, COLOR_GREEN, COLOR_BLACK); // Fruit color
    init_pair(3, COLOR_WHITE, COLOR_BLACK); // Border color
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            if (i == 0 || j == 0 || i == width - 1 || j == height - 1)
            {
                attron(COLOR_PAIR(3));
                mvprintw(j, i, "#");
                attroff(COLOR_PAIR(3));
            }
            else if (i == headX && j == headY)
            {
                attron(COLOR_PAIR(1));
                mvprintw(j, i, "W");
                attroff(COLOR_PAIR(1));
            }
            else if (i == fruitX && j == fruitY)
            {
                attron(COLOR_PAIR(2));
                mvprintw(j, i, "o");
                attroff(COLOR_PAIR(2));
            }
            else
            {
                bool print = false;
                for (int z = 0; z < tailLength; z++)
                {
                    if (tailX[z] == i && tailY[z] == j)
                    {
                        attron(COLOR_PAIR(4));
                        mvprintw(j, i, "w");
                        print = true;
                        attroff(COLOR_PAIR(4));
                    }
                }
                if (!print)
                    mvprintw(j, i, " ");
            }
        }
    }
    mvprintw(height, 0, "Current score : %d", score);
    mvprintw(height + 1, 0, "Highest score : %d", highestScore);

    refresh();
}

void logic()
{
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = headX;
    tailY[0] = headY;
    for (int i = 1; i < tailLength; i++)
    {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (d)
    {
    case stop:
        break;
    case leftss:
        headX--;
        break;
    case rightss:
        headX++;
        break;
    case up:
        headY--;
        break;
    case down:
        headY++;
        break;
    default:
        break;
    }

    if (headX >= width || headX <= 0 || headY >= height || headY <= 0)
        gameOver = true;

    for (int i = 0; i < tailLength; i++)
    {
        if (tailX[i] == headX && tailY[i] == headY)
            gameOver = true;
    }

    if (headX == fruitX && headY == fruitY)
    {
        tailLength++;
        score++;
        if (score > highestScore)
        {
            highestScore = score;
        }
        fruitX = rand() % (width - 3) + 1;
        fruitY = rand() % (height - 3) + 1;
    }
}

void input()
{
    int ch = getch();
    switch (ch)
    {
    case 'w':
        d = up;
        break;
    case 'a':
        d = leftss;
        break;
    case 'd':
        d = rightss;
        break;
    case 's':
        d = down;
        break;
    case 'e':
        gameOver = true;
        break;
    default:
        break;
    }
}

int main()
{
    initscr();
    clear();
    noecho();
    cbreak();
    timeout(100);
    keypad(stdscr, TRUE);

    height = 20;
    width = 30;
    cout << "Choose difficulty (1, 2, 3)   :";
    int diff;
    cin >> diff;
    setup();
    int timer;
    // s filename = "highscore.txt";
    getHigh();
    if (diff == 1)
    {
        timer = 100000;
    }
    else if (diff == 2)
    {
        timer = 50000;
    }
    else
    {
        timer = 1000;
    }

    while (!gameOver)
    {
        draw();
        input();
        logic();
        usleep(timer);
    }
    writeHigh();

    endwin();
    return 0;
}
