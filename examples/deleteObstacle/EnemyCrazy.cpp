#include "EnemyCrazy.h"

EnemyCrazy::EnemyCrazy(WINDOW * w, int a, int b): Enemy ( w, a, b ), changeCounter(3)
{
    coords.emplace_back(x,y);
    coords.emplace_back(x+1,y);
    coords.emplace_back(x+2,y);
    coords.emplace_back(x+1,y+1);
    showMe();
}

void EnemyCrazy::change()
{
    changeCounter = 3;
    if ( coords[0].second == coords[1].second )
    {
        coords[0].second++;
        coords[2].second++;
    }
    else
    {
        coords[0].second--;
        coords[2].second--;
    }
}

unique_ptr<Bullet> EnemyCrazy::shoot()
{
    if ( rand() % 2 )
        return make_unique<BulletBasic> (BulletBasic(win,x+1,y+2,false));
    return make_unique<BulletRandom> (BulletRandom(win, x + 1, y + 2, false));
}

void EnemyCrazy::moveRight()
{
    x++;
    for ( auto & i : coords )
    {
        mvwprintw(win,i.second,i.first," ");
        i.first++;
    }
    if ( !--changeCounter )
        change();
    showMe();
}

void EnemyCrazy::moveLeft()
{
    x--;
    for ( auto & i : coords )
    {
        mvwprintw(win,i.second,i.first," ");
        i.first--;
    }
    if ( !--changeCounter )
        change();
    showMe();
}