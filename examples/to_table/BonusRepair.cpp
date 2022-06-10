#include "BonusRepair.h"

void BonusRepair::applyBonus(SpaceShip *spaceship, EnemyArmy *enemyArmy,
                             list<unique_ptr<Obstacle>> &obstacles)
{
    repairObstacles(obstacles);
}

void BonusRepair::repairObstacles( list<unique_ptr<Obstacle>> &obstacles )
{
    for ( const auto & i : obstacles )
        i->repair();
}