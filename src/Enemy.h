#ifndef ENEMY_H
#define ENEMY_H

#include "imports.cpp"

#include "Object.h"
#include "Bullet.h"
#include "BulletRandom.h"
using namespace std;

/**
 * This class represents one of our enemies. In this program we use subclasses of Enemies.
 * Enemy can move down and shoot.
 */

class Enemy:public Object{
public:
                                Enemy       ( WINDOW * w, int a, int b );
                                ~Enemy      ();
    void                        getDown     ();
    unique_ptr<Bullet> virtual  shoot       () {return nullptr;}
};

#endif
