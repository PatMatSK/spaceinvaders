#include "BonusShield.h"

void BonusShield::applyBonus(SpaceShip *spaceShip, EnemyArmy *enemyArmy,
                             list<unique_ptr<Obstacle>> &obstacles)
{
    if ( !spaceShip->shield )
        spaceShip->activateShield();
}
