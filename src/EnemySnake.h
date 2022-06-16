#ifndef ENEMYSNAKE_H
#define ENEMYSNAKE_H

#include "imports.cpp"
#include "Enemy.h"
#include "Bullet.h"
#include "BulletRandom.h"
/*
 * Subclass of enemy, that shoot only random bullets and moves like snake ( zig-zag ).
 */
class EnemySnake: public Enemy {
public:
                        EnemySnake  ( WINDOW * w, int a, int b );
    unique_ptr<Bullet>  shoot       () override;
    void                moveLeft    () override;
    void                moveRight   () override;
};

#endif
