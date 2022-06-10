#include "BonusStop.h"


void BonusStop::applyBonus(SpaceShip *spaceship, EnemyArmy *enemyArmy,
                           list<unique_ptr<Obstacle>> &obstacles)
{
    enemyArmy->ableToMove = false;
}