#ifndef ENEMY_H
#define ENEMY_H

#include "imports.cpp"

#include "Object.h"
#include "Bullet.h"

using namespace std;

class Enemy:public Object{
public:
                        Enemy( WINDOW * w, int a, int b );
                        ~Enemy      ();
    void                getDown     ();
    unique_ptr<Bullet>  shoot       () const;
};

#endif