#ifndef ENEMYBASIC_H
#define ENEMYBASIC_H

#include "imports.cpp"
#include "Enemy.h"

class EnemyBasic: public Enemy {
public:
            EnemyBasic( WINDOW * w, int a, int b );
    void                moveLeft    () override;
    void                moveRight   () override;
};


#endif
