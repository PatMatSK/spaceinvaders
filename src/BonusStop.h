#ifndef BONUSSTOP_H
#define BONUSSTOP_H

#include "imports.cpp"
#include "Bonus.h"
#include "SpaceShip.h"
#include "EnemyArmy.h"
#include "Obstacle.h"
/**
 * Bonus that stops enemies for short time.
 */
class BonusStop: public Bonus
{
public:
    void    applyBonus( SpaceShip * spaceShip, EnemyArmy * enemyArmy,
                        list<unique_ptr<Obstacle>> & obstacles ) override;
};


#endif
