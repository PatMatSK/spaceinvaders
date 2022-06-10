#include "BulletRandom.h"

BulletRandom::BulletRandom(WINDOW *w, int a, int b, bool d): Bullet(w,a,b,d) {}

bool BulletRandom::moveOne( int a, int b )
{
    coords[0].first = a;
    coords[0].second = b;
    if ( canMove(a,b) )
    {
        mvwprintw(win,y,x," ");
        x = a;
        y = b;
        showMe();
        wrefresh(win);
        return true;
    }
    if ( direction )
        coords[0].second++;
    else
        coords[0].second--;

    return false;
}

bool BulletRandom::move( bool direction )
{
    bool myWay = rand() % 2;
    if ( myWay )
    {
        if ( direction )
            return moveOne(x-1,y-1);
        return moveOne(x-1,y+1);
    }
    else
    {
        if ( direction )
            return moveOne(x+1,y-1);
        return moveOne(x+1,y+1);
    }
}
