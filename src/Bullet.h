#ifndef BULLET_H
#define BULLET_H

#include "Object.h"
/*
 * This class is main skeleton of Bullet. In this program we use certain subclasses of Bullet.
 */
using namespace std;

class Bullet: public Object{
protected:
    bool            direction;
public:
                    Bullet      ( WINDOW *w, int a, int b, bool d);
                    ~Bullet     ();
    pair<int,int>   getCoords   (){ return coords[0]; }
    bool            canMove     ( int a, int b ) override;
};

#endif