#ifndef BONUSREPAIR_H
#define BONUSREPAIR_H

#include "imports.cpp"
#include "Bonus.h"
#include "SpaceShip.h"
#include "EnemyArmy.h"
#include "Obstacle.h"

class BonusRepair: public Bonus
{
public:
    void    repairObstacles ( list<unique_ptr<Obstacle>> & obstacles );
    void    applyBonus      ( SpaceShip * spaceShip, EnemyArmy * enemyArmy,
                              list<unique_ptr<Obstacle>> & obstacles ) override;
};


#endif
