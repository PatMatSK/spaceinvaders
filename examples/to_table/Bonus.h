#ifndef BONUS_H
#define BONUS_H

#include "imports.cpp"
#include "SpaceShip.h"
#include "EnemyArmy.h"
#include "Obstacle.h"

class Bonus {
public:
    void virtual    applyBonus( SpaceShip * spaceship, EnemyArmy * enemyArmy, list<unique_ptr<Obstacle>> & obstacles ){}
};


#endif
