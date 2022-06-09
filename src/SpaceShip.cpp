#include "SpaceShip.h"



SpaceShip::SpaceShip( int a, int b, WINDOW * w ): Object(w,"o"),width(a),shield(false)
{
    x = a/2 - 5;
    y = b - 4;
    for ( int i = 0; i < 9; i++ )
    {
        coords.emplace_back(x+i,y);
        coords.emplace_back(x+i,y-1);
    }
    coords.emplace_back(x+3,y-2);
    coords.emplace_back(x+4,y-2);
    coords.emplace_back(x+5,y-2);
    showMe();
}

SpaceShip::~SpaceShip()
{
    for ( const auto & i : coords )
        mvwprintw(win,i.second,i.first," ");
}


bool SpaceShip::canMove( int a, int b )
{
    if (  a <= 0 || a + 9 >= width )
        return true;
    return false;
}

Bullet* SpaceShip::shoot()
{
    return new Bullet(win,x+4,y-3,true);
}

void SpaceShip::moveRight()
{
    if (  canMove(x+1, y) )
        return;

    for (auto & coord : coords)
        coord.first++;
    x++;
    showMe();
    mvwprintw(win,coords[0].second, coords[0].first-1," ");
    mvwprintw(win,coords[1].second, coords[1].first-1," ");
    mvwprintw(win,coords[18].second, coords[18].first-1," ");
    wrefresh(win);
}

void SpaceShip::moveLeft()
{
    if ( canMove(x-1,y) )
        return;

    for (auto & coord : coords)
        coord.first--;
    x--;
    showMe();
    mvwprintw(win,coords[16].second, coords[16].first+1," ");
    mvwprintw(win,coords[17].second, coords[17].first+1," ");
    mvwprintw(win,coords[20].second, coords[20].first+1," ");
    wrefresh(win);
}

void SpaceShip::activateShield()
{
    me = "O";
    shield = true;
    showMe();
}

void SpaceShip::deactivateShield()
{
    me = "o";
    shield = false;
    showMe();
}