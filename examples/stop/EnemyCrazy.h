#ifndef ENEMYCRAZY_H
#define ENEMYCRAZY_H

#include "imports.cpp"
#include "Enemy.h"
#include "Bullet.h"
#include "BulletRandom.h"
#include "BulletBasic.h"

class EnemyCrazy: public Enemy {
private:
    int         changeCounter;
public:
                        EnemyCrazy  ( WINDOW * w, int a, int b );
    unique_ptr<Bullet>  shoot       () override;
    void                moveLeft    () override;
    void                moveRight   () override;
    void                change      ();
};

#endif
