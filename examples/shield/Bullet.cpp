#include "Bullet.h"

using namespace std;

Bullet::Bullet(WINDOW *w, int a, int b, bool d) : Object(w,"|"), direction(d)
{
    x = a;
    y = b;
    coords.emplace_back(a,b);
    showMe();
}

Bullet::~Bullet()
{
    mvwprintw( win, y, x, " " );
    wrefresh(win);
}

bool Bullet::canMove(int a, int b)
{
    auto  target = chtype mvwinch( win, b, a);
    if (  target == 'X' && ! direction  )   // no hitting enemy by enemy
        return true;
    return target == ' ';
}
