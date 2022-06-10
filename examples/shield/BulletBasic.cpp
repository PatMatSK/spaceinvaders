#include "BulletBasic.h"

BulletBasic::BulletBasic(WINDOW *w, int a, int b, bool d): Bullet(w,a,b,d) {}

bool BulletBasic::move( bool direction )
{
    int old_y = coords[0].second;
    if ( direction && canMove( coords[0].first, --coords[0].second ) )
    {
        showMe();
        mvwprintw(win,coords[0].second+1,coords[0].first," ");
        wrefresh(win);
        y--;
        return true;
    }
    else if ( canMove( coords[0].first, ++coords[0].second ) )
    {
        showMe();
        mvwprintw(win,coords[0].second-1,coords[0].first," ");
        wrefresh(win);
        y++;
        return true;
    }
    coords[0].second = old_y;
    return false;
}
