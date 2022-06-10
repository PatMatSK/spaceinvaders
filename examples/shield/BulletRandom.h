#ifndef BULLETAVOID_H
#define BULLETAVOID_H

#include "imports.cpp"
#include "Bullet.h"

class BulletRandom: public Bullet {
public:
                    BulletRandom     (WINDOW *w, int a, int b, bool d);
    bool            moveOne         ( int a, int b );
    bool            move            ( bool direction ) override ;
};

#endif
