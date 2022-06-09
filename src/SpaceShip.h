#ifndef SPACESHIP_H
#define SPACESHIP_H

#include "imports.cpp"

#include "Object.h"
#include "Bullet.h"

using namespace std;

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