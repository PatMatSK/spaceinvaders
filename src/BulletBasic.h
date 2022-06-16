#ifndef BULLETBASIC_H
#define BULLETBASIC_H

#include "imports.cpp"
#include "Bullet.h"
/**
 * Basic bullet that moves only forward.
 */
class BulletBasic: public Bullet {
public:
                    BulletBasic     ( WINDOW *w, int a, int b, bool d );
    bool            move            ( bool direction ) override ;
};

#endif
