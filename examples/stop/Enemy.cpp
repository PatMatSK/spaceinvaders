#include "Enemy.h"
using namespace std;

Enemy::Enemy(WINDOW * w, int a, int b): Object(w,"X")
{
    x = a; y = b;
    coords.emplace_back(x,y);
    coords.emplace_back(x+1,y);
    coords.emplace_back(x+2,y);
    coords.emplace_back(x,y+1);
    coords.emplace_back(x+2,y+1);
    showMe();
}

Enemy::~Enemy()
{
    for (const auto &i: coords)
        mvwprintw(win, i.second, i.first, " ");
}

unique_ptr<Bullet>  Enemy::shoot() const
{
    return make_unique<Bullet> (Bullet(win,x+1,y+2,false));
}

void Enemy::move( bool direction )
{
    if ( direction )
        moveRight();
    else
        moveLeft();

    showMe();
    wrefresh(win);
}

void Enemy::getDown()
{
    mvwprintw(win,coords[0].second,coords[0].first,"   ");
    for ( auto & i : coords )
        i.second++;
    y++;
    showMe();
}

void Enemy::moveLeft()
{
    for ( auto & i : coords)
        i.first--;
    x--;
    mvwprintw(win,coords[2].second,coords[2].first+1," ");
    mvwprintw(win,coords[3].second,coords[3].first+1," ");
    mvwprintw(win,coords[4].second,coords[4].first+1," ");
}

void Enemy::moveRight()
{
    for ( auto & i : coords)
        i.first++;
    x++;
    mvwprintw(win,coords[0].second,coords[0].first-1," ");
    mvwprintw(win,coords[3].second,coords[3].first-1," ");
    mvwprintw(win,coords[4].second,coords[4].first-1," ");
}
