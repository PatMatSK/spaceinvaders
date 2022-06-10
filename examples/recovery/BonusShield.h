#ifndef BONUSSHIELD_H
#define BONUSSHIELD_H

#include "imports.cpp"
#include "Bonus.h"
#include "SpaceShip.h"
#include "EnemyArmy.h"
#include "Obstacle.h"

class BonusShield: public Bonus
{
public:
    void    applyBonus( SpaceShip * spaceShip, EnemyArmy * enemyArmy,
                        list<unique_ptr<Obstacle>> & obstacles ) override;
};


#endif
