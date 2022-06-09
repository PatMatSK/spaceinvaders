#ifndef ENEMYSNAKE_H
#define ENEMYSNAKE_H

#include "imports.cpp"
#include "Enemy.h"

class EnemySnake: public Enemy {
public:
            EnemySnake( WINDOW * w, int a, int b );
    void                moveLeft    () override;
    void                moveRight   () override;
};

#endif
