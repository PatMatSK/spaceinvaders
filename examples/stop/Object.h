#ifndef OBJECT_H
#define OBJECT_H


#include "imports.cpp"
using namespace std;

class Object{
protected:
    WINDOW *                win;
    vector<pair<int,int>>   coords;
    int                     x;
    int                     y;
    const char *            me;
public:
                            Object          ( WINDOW * w, const char * m ): win(w),me(m){}
    virtual bool            canMove         ( int a, int b){return false;}
    bool                    contains        ( const pair<int,int> & c);
    bool virtual            move            ( bool direction );
    void virtual            moveRight       (){}
    void virtual            moveLeft        (){}
    void                    showMe          ();
};

#endif
