#include <ncurses.h>
#include <iostream>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include <vector>


#define TIMEOUT 5

using namespace std;


//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

class Object{
protected:
    WINDOW * win;
public:
    Object(WINDOW * w){
        win = w;
    }
    //virtual void gotHit() ;
    virtual bool canMove(int x, int y){return false;}
    virtual void move( bool direction ){}

};


//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
class Bullet: public Object{
private:
    int x;
    int y;
    bool direction;   // true == up;   false == down

public:
    Bullet(WINDOW *w, int a, int b, bool d) : Object(w), x(a), y(b), direction(d) {
        mvwprintw( win, y, x, "|" );
    }
    ~Bullet(){
        mvwprintw( win, y+1, x, " " );
    }
    bool canMove(int x, int y) override {
        char daco = chtype mvwinch( win, y, x);
        if ( daco != ' ' )
            return false;
        return true;
    }
    bool move ()
    {
        if ( direction ){
            if  ( canMove( x, --y ) ){
                mvwprintw(win,y,x,"|");
                mvwprintw(win,y+1,x," ");
                wrefresh(win);
                return true;
            }
        }
        else{
            if  ( canMove( x, ++y ) ){
                mvwprintw(win,y,x,"|");
                mvwprintw(win,y-1,x," ");
                wrefresh(win);
                return true;
            }
        }
        return false;
    }
};



//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

class Enemy:public Object{
private:
    int x;
    int y;
    vector<pair<int,int>> coords;
public:
    Enemy(WINDOW * w, int a, int b): Object(w), x(a), y(b) {
        coords.emplace_back(x,y);
        coords.emplace_back(x+1,y);
        coords.emplace_back(x+2,y);
        coords.emplace_back(x,y+1);
        coords.emplace_back(x+2,y+1);
        showEnemy();
    }
    void showEnemy(){
        for ( const auto & i: coords )
            mvwprintw(win,i.second,i.first,"X");
    }
    bool canMove(int x, int y) override{
        if ( x <= 0 || x + 4 >= 100)
            return false;
        return true;
    }
    void move( bool direction ) override {

        if ( direction ){
            if ( !canMove(x+1,y) )
                return;
            moveRight();
        }
        else{
            if ( !canMove(x-1,y) )
                return;
            moveLeft();
        }

        showEnemy();
        wrefresh(win);

    }
    void moveLeft();
    void moveRight();


};
void Enemy::moveLeft() {
    for ( auto & i : coords)
        i.first--;
    //for ( auto  i : coords)
      //  i.first--;

    mvwprintw(win,coords[2].second,coords[2].first+1," ");
    mvwprintw(win,coords[3].second,coords[3].first+1," ");
    mvwprintw(win,coords[4].second,coords[4].first+1," ");

}


void Enemy::moveRight() {
    for ( auto & i : coords)
        i.first++;
    //for ( auto  i : coords)
      //  i.first++;
    mvwprintw(win,coords[0].second,coords[0].first-1," ");
    mvwprintw(win,coords[3].second,coords[3].first-1," ");
    mvwprintw(win,coords[4].second,coords[4].first-1," ");

}



//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

class EnemyArmy{
private:
    vector<Enemy*> enemies;
    bool direction; // true == right, false == left
    int leftEdge;
    int rightEdge;
public:

    EnemyArmy( WINDOW * win, int count ){
        for ( int i = 1 ; i < count+1 ; i++ )
            for ( int j = 0 ; j < 15 ; j+=3 )
                enemies.push_back(new Enemy( win,i*5 + 10, j + 10 ));
        direction = true;
        leftEdge = 15;
        rightEdge = 15 + count * 5;
        wrefresh( win );
    }
    void moveArmy();
    bool isAlive(){ return !enemies.empty(); }

};

void EnemyArmy::moveArmy() {
    if ( rightEdge >= 100)
        direction = false;
    if ( leftEdge <= 1 )
        direction = true;

    for ( auto & i : enemies )
        i->move(direction);
    if ( direction ){
        leftEdge++;
        rightEdge++;
    }
    else{
        leftEdge--;
        rightEdge--;
    }
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
class SpaceShip : public Object {
private:
    int x;      //anchor left down corner
    int y;
    vector<pair<int,int>> coords;
public:
    SpaceShip( int a, int b, WINDOW * w ): Object(w),x(a/2-5),y(b-4){
        for ( int i = 0; i < 9; i++ ){
            coords.emplace_back(x+i,y);
            coords.emplace_back(x+i,y-1);
        }
        coords.emplace_back(x+3,y-2);
        coords.emplace_back(x+4,y-2);
        coords.emplace_back(x+5,y-2);
        showSapceShip();
    }
    void showSapceShip(){
        for ( const auto & i : coords )
            mvwprintw(win,i.second,i.first,"o");
    }


    bool canMove( int x, int y ) override {

        if (  x <= 0 || x >= 90 )
            return true;
        return false;
    }

    void moveLeft(){
        if ( canMove(x-1,y) )
            return;

        for (auto & coord : coords)
            coord.first--;
        x--;
        showSapceShip();
        mvwprintw(win,coords[16].second, coords[16].first+1," ");
        mvwprintw(win,coords[17].second, coords[17].first+1," ");
        mvwprintw(win,coords[20].second, coords[20].first+1," ");
        wrefresh(win);
    }
    void moveRight(){
        if (  canMove(x+1, y) )
            return;

        for (auto & coord : coords)
            coord.first++;
        x++;
        showSapceShip();
        mvwprintw(win,coords[0].second, coords[0].first-1," ");
        mvwprintw(win,coords[1].second, coords[1].first-1," ");
        mvwprintw(win,coords[18].second, coords[18].first-1," ");
        wrefresh(win);

        wrefresh(win);
    }

   Bullet * shoot(){
        return new Bullet(win,x+4,y-3,true);
    }



};

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------




class Level{

private:
    int level;
    SpaceShip * spaceShip;
    Bullet  * myBullet;
    EnemyArmy * enemyArmy;
    WINDOW  * win;
    int height,width;
public:
    bool wannaContinue;
    Level(int l ):level(l),wannaContinue(true), myBullet(nullptr), height(50), width(100)
    {
        initscr();
        win = newwin(height,width,0,0);
        refresh();
        box(win,0,0);
        enemyArmy = new EnemyArmy(win, 10);
        spaceShip = new SpaceShip(width,height,win);
        wrefresh(win);
        refresh();
    }
    ~Level(){
        delete myBullet;
        delete spaceShip;
        delete enemyArmy;
    }
    void play();

};


void Level::play(){
    noecho();

    while ( enemyArmy->isAlive() && spaceShip != nullptr )
    {

        char a  = getch();

        if ( a == 'a' )
            spaceShip->moveLeft();
        else if ( a == 'd' )
            spaceShip->moveRight();
        else if ( a == 'm' && myBullet == nullptr )
            myBullet = spaceShip->shoot();


        if ( myBullet )
            if ( ! myBullet->move() ){
                delete myBullet;
                myBullet = nullptr;
            }
        enemyArmy->moveArmy();

    }
}



//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
int main() {
    int i = 0;
    bool wantContinue = true;

    while (wantContinue)
    {
        Level * lvl = new Level(++i);
        lvl->play();
        wantContinue = lvl->wannaContinue;
        endwin();
        delete lvl;
    }

    return 0;
}
