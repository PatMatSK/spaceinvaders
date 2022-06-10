#include "Enemy.h"
using namespace std;

Enemy::Enemy(WINDOW * w, int a , int b ): Object(w,"X")
{
    x = a;
    y = b;
}

Enemy::~Enemy()
{
    for (const auto &i: coords)
        mvwprintw(win, i.second, i.first, " ");
}

void Enemy::getDown()
{
    for ( auto & i : coords )
    {
        mvwprintw(win,i.second,i.first," ");
        i.second++;
    }
    y++;
    showMe();
}
