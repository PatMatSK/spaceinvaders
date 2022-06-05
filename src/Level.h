#ifndef LEVEL_H
#define LEVEL_H

#include "imports.cpp"

#include "Object.h"
#include "EnemyArmy.h"
#include "SpaceShip.h"
#include "Bullet.h"
#include "Obstacle.h"

using namespace std;

class Level{
private:
    SpaceShip *                 spaceShip;
    Bullet *                    myBullet;
    EnemyArmy *                 enemyArmy;
    WINDOW *                    win;
    list<unique_ptr<Obstacle>>  obstacles;
    list<unique_ptr<Bullet>>    enemyBullets;
    int                         height;
    int                         width;
    string                      getNickName         ( const pair<int,int>& position );
    void                        intro               ();
    void                        scoreIncrease       ();
    void                        playerDied          ();
    void                        obstacleCreator     ();
    void                        ask                 ();
    void                        writePlayer         ();
    void                        saveObstacles       ();
    void                        obstacleReader      ();
    void                        moveBullets         ();
    void                        moveSpaceship       ();
    void                        hitByMyBullet       ();
    bool                        hitByEnemyBullet    ( const pair<int,int> & c );
    void                        bulletHitted        ( const pair<int,int> & c);
    bool                        obstacleHitted      ( const pair<int,int> & c);
    void                        repairObstacles     ();
    void                        bonusCheck          ();
    void                        makeBonus           ();
public:
    int                         level;
    int                         score;
    int                         lives;
    bool                        fromStart;
    bool                        wannaContinue;
                                Level               ( int _lvl, int _score, int _lives, int _width, int _height );
                                ~Level              ();
    void                        play                ();

};

#endif