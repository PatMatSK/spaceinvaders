#ifndef SPACESHIP_H
#define SPACESHIP_H

#include "imports.cpp"
#include "Object.h"
#include "Bullet.h"
#include "BulletBasic.h"
using namespace std;

/**
 * This class represents our spaceShip, we can move left, right or shoot basic bullet.
 * Bonus can say to activate shield, and level to deactivate shield, if we got one.
 */

class SpaceShip : public Object {
private:
    int                 width;
public:
    bool                shield;
                        SpaceShip           ( int a, int b, WINDOW * w );
                        ~SpaceShip          ();
    bool                canMove             ( int a, int b ) override;
    void                moveLeft            () override;
    void                moveRight           () override ;
    Bullet *            shoot               ();
    void                activateShield      ();
    void                deactivateShield    ();
};

#endif
