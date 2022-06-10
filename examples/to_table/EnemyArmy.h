#ifndef ENEMYARMY_H
#define ENEMYARMY_H

#include "imports.cpp"

#include "Enemy.h"
#include "EnemyBasic.h"
#include "EnemySnake.h"
#include "EnemyCrazy.h"
#include "Bullet.h"

using namespace std;

class EnemyArmy{
private:
    list<unique_ptr<Enemy>> enemies;
    int                     bounceCount;
    bool                    direction; // true == right, false == left
    int                     width;
    int                     leftEdge;
    int                     rightEdge;
    int                     downEdge;   // donw edge of army
public:
    int                     downLimit;  // down limit influenced by obstacles and spaceship
    bool                    ableToMove;
                            EnemyArmy       ( WINDOW * win, int count , int _width, int _limit );
                            ~EnemyArmy      ();
    bool                    isAlive         (){ return !enemies.empty(); }
    void                    moveArmy        ();
    bool                    killEnemy       ( pair<int,int> coords );
    void                    getCloser       ();
    void                    checkChange     ();
    unique_ptr<Bullet>      enemyFire       () const ;
};

#endif