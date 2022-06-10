#ifndef MAIN_H
#define MAIN_H

#include "imports.cpp"
#include "Level.h"

using namespace std;

int main()
{
    bool wantContinue = true;
    int level = 0;
    int score = 5000;
    int livesCount = 1;
    int width = 50;
    int height = 29;
    initscr();
    refresh();

    while ( wantContinue )
    {
        Level * lvl = new Level(++level, score, livesCount, width, height);
        lvl->play();
        wantContinue = lvl->wannaContinue;
        if ( lvl->fromStart )
        {
            score = 5000;
            livesCount = 1;
            level = 0;
        }
        else
        {
            score = lvl->score;
            livesCount = lvl->lives;
            level = lvl->level;
        }
        delete lvl;
    }
    endwin();
    return 0;
}

#endif