#include "Obstacle.h"

using namespace std;

Obstacle::Obstacle(WINDOW *w, vector<pair<int, int>> v): Object(w,"Q")
{
    coords = std::move(v);
    origin = coords;
    showMe();
}

Obstacle::Obstacle(WINDOW *w, int a, int b): Object(w,"Q")
{
    coords.emplace_back(a,b);
    coords.emplace_back(a+1,b);
    coords.emplace_back(a+2,b);
    coords.emplace_back(a+3,b);
    coords.emplace_back(a+4,b);

    /*
    coords.emplace_back(a+7,b);
    coords.emplace_back(a+8,b);
    coords.emplace_back(a+9,b);
    for ( int i = 0; i < 10 ; i++ )
    {
        coords.emplace_back(a+i,b-1);
        coords.emplace_back(a+i,b-2);
    }
     */
    origin = coords;
    showMe();
}

void Obstacle::clean()
{
    for ( auto const & i : coords )
        mvwprintw(win,i.second,i.first," ");
    wrefresh(win);
}

 bool      Obstacle::getDamage( const pair<int,int> & c )
{
    coords.erase(find(coords.begin(), coords.end(), c) );
    mvwprintw(win,c.second,c.first," ");
    wrefresh(win);
    return coords.empty();
}
/*
 * Function to save obstacle to next level.
 */
 void Obstacle::writeCoords(ofstream &file)
{
    for ( const auto & i : coords )
        file << i.first << " " << i.second << endl;
    file << -1 << " " << -1 << endl;
}

void Obstacle::repair()
{
    coords = origin;
    showMe();
}