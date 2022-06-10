#include "EnemySnake.h"

EnemySnake::EnemySnake(WINDOW * w, int a, int b): Enemy (w,a,b)
{
    coords.emplace_back(x,y);
    coords.emplace_back(x+1,y+1);
    coords.emplace_back(x+2,y);
    showMe();
}

unique_ptr<Bullet>  EnemySnake::shoot()
{
    return make_unique<BulletRandom> (BulletRandom(win, x + 1, y + 2, false));
}

void EnemySnake::moveRight()
{
    x++;
    mvwprintw(win,coords[0].second,coords[0].first," ");
    wrefresh(win);
    coords.emplace_back(coords[0].first+3,coords[0].second);
    coords.erase(coords.begin());
}

void EnemySnake::moveLeft()
{
    x--;
    mvwprintw(win,coords[2].second,coords[2].first," ");
    wrefresh(win);
    coords.insert(coords.begin(),make_pair(coords[2].first-3,coords[2].second));
    coords.erase(coords.begin()+3);
}