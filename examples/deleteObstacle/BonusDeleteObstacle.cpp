#include "BonusDeleteObstacle.h"

void
BonusDeleteObstacle::applyBonus(SpaceShip *spaceShip, EnemyArmy *enemyArmy,
                                list<unique_ptr<Obstacle> > &obstacles)
{
    if ( ! obstacles.empty() )
    {
        auto it  = obstacles.begin();
        advance( it, rand() % obstacles.size() );
        (*it)->clean();
        obstacles.erase( it );
    }
}