#include <ncurses.h>
#include <iostream>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include <vector>
#include <Carbon/Carbon.h>
#include <list>
#include <fstream>
#include  <random>
#include  <iterator>


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
    virtual bool canMove(int x, int y){return false;}
    virtual void move( bool direction ){}
    virtual bool contains( pair<int,int> & c){return false;}
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
        mvwprintw( win, y, x, " " );
        wrefresh(win);
    }
    pair<int,int> getCoords(){ return make_pair(x,y-1); }
    bool canMove(int x, int y) override {
        char daco = chtype mvwinch( win, y, x);
        if ( daco == 'X' && direction )             // looking for enemy
            return false;
        else if ( daco == 'o' && !direction )      // looking for myShip
            return false;
        if ( y <= 1 || y >= 100)
            return false;
        return true;
    }
    bool move();
};

bool Bullet::move(){
    if ( direction && canMove( x, --y ) ){
        mvwprintw(win,y,x,"|");
        mvwprintw(win,y+1,x," ");
        wrefresh(win);
        return true;
    }
    else if ( canMove( x, ++y ) ){
        mvwprintw(win,y,x,"|");
        mvwprintw(win,y-1,x," ");
        wrefresh(win);
        return true;
    }
    return false;
}

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
    ~Enemy(){
        for ( const auto & i: coords )
            mvwprintw(win,i.second,i.first," ");
        wrefresh(win);
    }
    void showEnemy(){
        for ( const auto & i: coords )
            mvwprintw(win,i.second,i.first,"X");
        wrefresh(win);
    }
    bool canMove(int x, int y) override{
        if ( x <= 0 || x + 4 >= 100)
            return false;
        return true;
    }
    bool contains( pair<int,int> & c) override;

    void moveLeft();
    void moveRight();
    void getDown();
    void move( bool direction ) override;
    Bullet * shoot() const;
};

Bullet * Enemy::shoot() const {
    return new Bullet(win,x+1,y+2,false);
}


bool Enemy::contains( pair<int,int> & c){
    if ( count(coords.begin(), coords.end(), c) )
        return true;
    return false;
}


void Enemy::move( bool direction ) {

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

void Enemy::getDown() {
    mvwprintw(win,coords[0].second,coords[0].first,"   ");
    for ( auto & i : coords )
        i.second++;
    showEnemy();
}



void Enemy::moveLeft() {
    for ( auto & i : coords)
        i.first--;
    x--;

    mvwprintw(win,coords[2].second,coords[2].first+1," ");
    mvwprintw(win,coords[3].second,coords[3].first+1," ");
    mvwprintw(win,coords[4].second,coords[4].first+1," ");

}


void Enemy::moveRight() {
    for ( auto & i : coords)
        i.first++;
    x++;
    mvwprintw(win,coords[0].second,coords[0].first-1," ");
    mvwprintw(win,coords[3].second,coords[3].first-1," ");
    mvwprintw(win,coords[4].second,coords[4].first-1," ");

}



//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

class EnemyArmy{
private:
    list<unique_ptr<Enemy>> enemies;
    bool direction; // true == right, false == left
    int leftEdge;
    int rightEdge;
    int bounceCount;
public:

    EnemyArmy( WINDOW * win, int count ):bounceCount(0),direction(true){
        for ( int i = 1 ; i < count+1 ; i++ )
            for ( int j = 0 ; j < 15 ; j+=3 )
                enemies.emplace_back(make_unique<Enemy>( win,i*5 + 10, j + 10 ));

        leftEdge = 15;
        rightEdge = 15 + count * 5;
        wrefresh( win );
    }
    bool isAlive(){ return !enemies.empty(); }
    void moveArmy();
    void killEnemy( pair<int,int> coords );
    void getCloser();
    void checkChange();
    Bullet *  enemyFire() const ;
};

Bullet * EnemyArmy::enemyFire() const {
    auto it = enemies.cbegin();
    int random = rand() % enemies.size();
    advance(it, random);
    return (*it)->shoot();
}


void EnemyArmy::checkChange() {
    if ( rightEdge >= 100){
        direction = false;
        bounceCount++;
    }
    if ( leftEdge <= 1 ){
        direction = true;
        bounceCount++;
    }
    if ( bounceCount == 3 ){
        getCloser();
        bounceCount = 0;
    }


}


void EnemyArmy::getCloser() {
    for ( auto & i : enemies )
        i->getDown();
}



void EnemyArmy::moveArmy() {
    checkChange();

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


void EnemyArmy::killEnemy( pair<int,int> coords ) {

    for ( auto i = enemies.begin() ; i != enemies.end() ; i++ ){
        if ( (*i)->contains(coords) ){
            enemies.erase(i);
            return;
        }
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
    int score;
    int level;
    SpaceShip * spaceShip;
    Bullet  * myBullet;
    EnemyArmy * enemyArmy;
    WINDOW  * win;
    list<Bullet*> enemyBullets;
    int height,width;
    int lives;
    void intro();
public:
    bool wannaContinue;
    Level(int l, int s ):level(l),wannaContinue(true), myBullet(nullptr), height(50), width(100), lives(3)
    {
        score = s;
        initscr();
        win = newwin(height,width,0,0);
        refresh();
        box(win,0,0);
        intro();
        box(win,0,0);
        enemyArmy = new EnemyArmy(win, 10);
        spaceShip = new SpaceShip(width,height,win);
        mvwprintw(win,2,2,"SCORE: %d", score);

        mvwprintw(win,height-1,2,"LIVES: %d", lives);
        wrefresh(win);
        refresh();
    }
    ~Level(){
        delete myBullet;
        delete spaceShip;
        delete enemyArmy;
    }
    void play();
    int getScore(){ return score; }
    void scoreIncrease();
    void moveBullets();
};



void Level::intro() {
    ifstream file;
    file.open("top_five.txt");
    string nickname;
    int i = 0;
    mvwprintw(win,height/2-4,width/2-7,"SPACE INVADERS");
    mvwprintw(win,height/2-2,width/2-17,"top 5 players:");

    while ( getline(file, nickname)  )
        mvwprintw(win,height/2+i++,width/2-3,nickname.c_str());


    mvwprintw(win,height/2+10,width/2-7,"(press any key to start)");
    move(0,0);
    wrefresh(win);
    getch();
    wclear(win);
    file.close();
}


void Level::scoreIncrease() {
    score += 50;
    mvwprintw(win,2,2,"SCORE: %d", score);
    wrefresh(win);
}

void Level::moveBullets() {
    if ( myBullet )
        if ( ! myBullet->move() ){
            enemyArmy->killEnemy( myBullet->getCoords() );
            scoreIncrease();
            delete myBullet;
            myBullet = nullptr;
        }
    for ( auto i  = enemyBullets.begin(); i != enemyBullets.end(); i++){        //----------------nedokonane------------------------------
        if ( !(*i)->move() ){
            delete spaceShip;
            if ( --lives ){
                spaceShip = new SpaceShip(width,height,win);
                mvwprintw(win,height-1,2,"LIVES: %d", lives);
            }
        }
    }
}

void Level::play(){

    noecho();
    nodelay(stdscr, TRUE);
    int shoot = 20;
    while ( enemyArmy->isAlive() && spaceShip != nullptr )
    {
        char a  ;
        if ( ( a = getch()) == ERR )
            a = 'q' ;
        if ( a == 'a' )
            spaceShip->moveLeft();
        else if ( a == 'd' )
            spaceShip->moveRight();
        else if ( a == 'm' && myBullet == nullptr )
            myBullet = spaceShip->shoot();

        moveBullets();
        enemyArmy->moveArmy();

        if ( ! shoot-- ){
            enemyBullets.emplace_back(enemyArmy->enemyFire());
            shoot = 10;
        }
        usleep(100000);
    }

}



//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
int main() {
    int i = 0;
    bool wantContinue = true;
    int score = 0;
    while (wantContinue)
    {
        Level * lvl = new Level(++i, score);
        lvl->play();
        wantContinue = lvl->wannaContinue;
        endwin();
        score = lvl->getScore();
        delete lvl;
    }

    return 0;
}
