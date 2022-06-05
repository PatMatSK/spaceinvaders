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
    bool                canMove             ( int x, int y ) override;
    void                moveLeft            ();
    void                moveRight           ();
    Bullet *            shoot               ();
    void                activateShield      ();
    void                deactivateShield    ();
};

#endif