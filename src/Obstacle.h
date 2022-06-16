
#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "imports.cpp"
#include "Object.h"
using namespace std;

/**
 * This class represents obstacle, that can be destroyed part by part or at once.
 */
class Obstacle: public Object{
private:
    vector<pair<int,int>> origin;
public:
                    Obstacle        ( WINDOW * w, int a, int b);
                    Obstacle        ( WINDOW * w, vector<pair<int,int>> v);
    bool            getDamage       ( const pair<int,int> & c);
    void            writeCoords     ( ofstream & file);
    void            repair          ();
    void            clean           ();
};

#endif
