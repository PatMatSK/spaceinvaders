#include "Object.h"

using namespace std;

bool Object::contains( const pair<int,int> & c)
{
    auto i = find(coords.begin(), coords.end(), c);
    if ( i != coords.end() )
        return true;
    return false;
}

void Object::showMe()
{
    for ( const auto & i: coords )
        mvwprintw(win,i.second,i.first,me);
    wrefresh(win);
}
