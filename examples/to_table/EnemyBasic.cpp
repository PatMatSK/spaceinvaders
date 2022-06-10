#include "EnemyBasic.h"

EnemyBasic::EnemyBasic(WINDOW * w, int a, int b): Enemy ( w, a, b )
{
    coords.emplace_back(x,y);
    coords.emplace_back(x+1,y);
    coords.emplace_back(x+2,y);
    coords.emplace_back(x,y+1);
    coords.emplace_back(x+2,y+1);
    showMe();
}

unique_ptr<Bullet>  EnemyBasic::shoot()
{
    return make_unique<BulletBasic> (BulletBasic(win,x+1,y+2,false));
}

void EnemyBasic::moveLeft()
{
    for ( auto & i : coords)
        i.first--;
    x--;
    mvwprintw(win,coords[2].second,coords[2].first+1," ");
    mvwprintw(win,coords[3].second,coords[3].first+1," ");
    mvwprintw(win,coords[4].second,coords[4].first+1," ");
}

void EnemyBasic::moveRight()
{
    for ( auto & i : coords)
        i.first++;
    x++;
    mvwprintw(win,coords[0].second,coords[0].first-1," ");
    mvwprintw(win,coords[3].second,coords[3].first-1," ");
    mvwprintw(win,coords[4].second,coords[4].first-1," ");
}