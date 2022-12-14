#ifndef ENEMYBASIC_H
#define ENEMYBASIC_H

#include "imports.cpp"
#include "Enemy.h"
#include "Bullet.h"
#include "BulletBasic.h"

class EnemyBasic: public Enemy {
public:
                        EnemyBasic  ( WINDOW * w, int a, int b );
    unique_ptr<Bullet>  shoot       () override;
    void                moveLeft    () override;
    void                moveRight   () override;
};


#endif
