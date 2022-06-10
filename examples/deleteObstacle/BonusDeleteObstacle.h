#ifndef BONUSDELETEOBSTACLE_H
#define BONUSDELETEOBSTACLE_H

#include "imports.cpp"
#include "Bonus.h"

class BonusDeleteObstacle: public Bonus
{
public:
    void    applyBonus      ( SpaceShip * spaceShip, EnemyArmy * enemyArmy,
                              list<unique_ptr<Obstacle>> & obstacles ) override;
};


#endif
