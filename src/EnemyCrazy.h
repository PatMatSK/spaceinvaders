#ifndef ENEMYCRAZY_H
#define ENEMYCRAZY_H

#include "imports.cpp"
#include "Enemy.h"

class EnemyCrazy: public Enemy {
private:
    int         changeCounter;
public:
                EnemyCrazy  ( WINDOW * w, int a, int b );
    void        moveLeft    () override;
    void        moveRight   () override;
    void        change      ();
};

#endif
