#include "Bullet.h"

using namespace std;

Bullet::Bullet(WINDOW *w, int a, int b, bool d) : Object(w,"|"), direction(d)
{
    coords.emplace_back(a,b);
    showMe();
}

Bullet::~Bullet()
{
    mvwprintw( win, coords[0].second, coords[0].first, " " );
    wrefresh(win);
}


bool Bullet::canMove(int x, int y)
{
    auto  daco = chtype mvwinch( win, y, x);
    if (  daco == 'X' && ! direction  )   // no hitting enemy by enemy
        return true;
    return daco == ' ';
}

bool Bullet::move()
{
    int old_y = coords[0].second;
    if ( direction && canMove( coords[0].first, --coords[0].second ) )
    {
        showMe();
        mvwprintw(win,coords[0].second+1,coords[0].first," ");
        wrefresh(win);
        return true;
    }
    else if ( canMove( coords[0].first, ++coords[0].second ) )
    {
        showMe();
        mvwprintw(win,coords[0].second-1,coords[0].first," ");
        wrefresh(win);
        return true;
    }
    coords[0].second = old_y;
    return false;
}
