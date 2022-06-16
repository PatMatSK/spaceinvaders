#ifndef ENEMYCRAZY_H
#define ENEMYCRAZY_H

#include "imports.cpp"
#include "Enemy.h"
#include "Bullet.h"
#include "BulletRandom.h"
#include "BulletBasic.h"
/*
 * Subclass of enemy, that randomly decides whether to shoot basic bullet or random bullet.
 * Moves by changing positions of side parts, that makes harder to hit.
 */
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
