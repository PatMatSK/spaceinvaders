// can write winner to file, but get points when hitting nothing
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
    vector<pair<int,int>> coords;
    int x;
    int y;
    const char * me;
public:
    Object(WINDOW * w, const char * m ): win(w),me(m){}
    virtual bool    canMove(int a, int b){return false;}
    virtual void    move( bool direction ){}
    bool            contains( const pair<int,int> & c);
    void            showMe();
};

bool Object::contains( const pair<int,int> & c){

    //if ( count(coords.begin(), coords.end(), c) )
    auto i = find(coords.begin(), coords.end(), c);
    if ( i != coords.end() )
        return true;
    return false;
}

void Object::showMe(){
    for ( const auto & i: coords )
        mvwprintw(win,i.second,i.first, me);
    wrefresh(win);

}


//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
class Bullet: public Object{
private:
    //int x;
    //int y;
    bool direction;   // true == up;   false == down

public:
    Bullet(WINDOW *w, int a, int b, bool d) : Object(w,"|"), direction(d) {
        coords.emplace_back(a,b);
        showMe();
    }
    ~Bullet(){
        mvwprintw( win, coords[0].second, coords[0].first, " " );
        wrefresh(win);
    }
    pair<int,int>   getCoords(){ return coords[0]; }
    bool            canMove(int x, int y) override;
    bool            move();
    unsigned int    bulletHit();

};




bool Bullet::canMove(int x, int y) {
    auto  daco = chtype mvwinch( win, y, x);
    if (  daco == 'X' && ! direction  )   // no hitting enemy by enemy
        return true;
    return daco == ' ';
}

bool Bullet::move(){
    int old_y = coords[0].second;
    if ( direction && canMove( coords[0].first, --coords[0].second ) ){
        //y--;
        showMe();
        mvwprintw(win,coords[0].second+1,coords[0].first," ");
        wrefresh(win);
        return true;
    }
    else if ( canMove( coords[0].first, ++coords[0].second ) ){
        //y++;
        showMe();
        mvwprintw(win,coords[0].second-1,coords[0].first," ");
        wrefresh(win);
        return true;
    }
    //if ( coords[0].second > old_y )
    coords[0].second = old_y;

    return false;
}

unsigned int Bullet::bulletHit(){
    if ( direction )
        return chtype mvwinch( win, coords[0].second-1, x);
    return chtype mvwinch( win, coords[0].second+1, x);
}


//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

class Obstacle: public Object{
public:
    Obstacle(WINDOW * w, int a, int b);
    bool    getDamage( const pair<int,int> & c);
};

Obstacle::Obstacle(WINDOW *w, int a, int b): Object(w,"Q") {
    x = a; y = b;
    coords.emplace_back(a,b);
    coords.emplace_back(a+1,b);
    coords.emplace_back(a+2,b);
    coords.emplace_back(a+7,b);
    coords.emplace_back(a+8,b);
    coords.emplace_back(a+9,b);
    for ( int i = 0; i < 10 ; i++ ){
        coords.emplace_back(a+i,b-1);
        coords.emplace_back(a+i,b-2);
    }
    showMe();
}

bool      Obstacle::getDamage( const pair<int,int> & c ){
    //auto i = find(coords.begin(), coords.end(), c);
    coords.erase(find(coords.begin(), coords.end(), c) );
    mvwprintw(win,c.second,c.first," ");
    wrefresh(win);
    return coords.empty();
}


//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

class Enemy:public Object{
private:

public:
    Enemy(WINDOW * w, int a, int b): Object(w,"X") {
        x = a; y = b;
        coords.emplace_back(x,y);
        coords.emplace_back(x+1,y);
        coords.emplace_back(x+2,y);
        coords.emplace_back(x,y+1);
        coords.emplace_back(x+2,y+1);
        showMe();
    }
    ~Enemy(){
        for ( const auto & i: coords )
            mvwprintw(win,i.second,i.first," ");
        wrefresh(win);
    }

    void moveLeft();
    void moveRight();
    void getDown();
    void move( bool direction ) override;
    Bullet * shoot() const;
};

Bullet * Enemy::shoot() const {
    return new Bullet(win,x+1,y+2,false);
}



void Enemy::move( bool direction ) {

    if ( direction )
        moveRight();
    else
        moveLeft();

    showMe();
    wrefresh(win);

}

void Enemy::getDown() {
    mvwprintw(win,coords[0].second,coords[0].first,"   ");
    for ( auto & i : coords )
        i.second++;
    y++;
    showMe();
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
    int width;
public:

    EnemyArmy( WINDOW * win, int count , int w ):bounceCount(0),direction(true),width(w){
        for ( int i = 1 ; i < count+1 ; i++ )
            for ( int j = 0 ; j < 6 ; j+=3 )
                enemies.emplace_back(make_unique<Enemy>( win,i*5 + 10, j + 5 ));

        leftEdge = 15;
        rightEdge = 15 + count * 5;
        wrefresh( win );
    }
    ~EnemyArmy(){
        enemies.clear();
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
    if ( rightEdge >= width ){
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
        if ( (*i)->contains(coords ) ){
            enemies.erase(i);
            return;
        }
    }
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
class SpaceShip : public Object {
private:
    int width;
public:
    SpaceShip( int a, int b, WINDOW * w ): Object(w,"O"),width(a){
        x = a/2 - 5;
        y = b - 4;
        for ( int i = 0; i < 9; i++ ){
            coords.emplace_back(x+i,y);
            coords.emplace_back(x+i,y-1);
        }
        coords.emplace_back(x+3,y-2);
        coords.emplace_back(x+4,y-2);
        coords.emplace_back(x+5,y-2);
        showMe();
    }
    ~SpaceShip(){
        for ( const auto & i : coords )
            mvwprintw(win,i.second,i.first," ");
    }

    bool canMove( int x, int y ) override {
        if (  x <= 0 || x + 9 >= width )
            return true;
        return false;
    }

    void moveLeft();
    void moveRight();

   Bullet * shoot(){
        return new Bullet(win,x+4,y-3,true);
    }
};


void SpaceShip::moveRight(){
    if (  canMove(x+1, y) )
        return;

    for (auto & coord : coords)
        coord.first++;
    x++;
    showMe();
    mvwprintw(win,coords[0].second, coords[0].first-1," ");
    mvwprintw(win,coords[1].second, coords[1].first-1," ");
    mvwprintw(win,coords[18].second, coords[18].first-1," ");
    wrefresh(win);

    wrefresh(win);
}

void SpaceShip::moveLeft(){
    if ( canMove(x-1,y) )
        return;

    for (auto & coord : coords)
        coord.first--;
    x--;
    showMe();
    mvwprintw(win,coords[16].second, coords[16].first+1," ");
    mvwprintw(win,coords[17].second, coords[17].first+1," ");
    mvwprintw(win,coords[20].second, coords[20].first+1," ");
    wrefresh(win);
}

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
    vector<Obstacle*> obstacles;
    list<shared_ptr<Bullet>> enemyBullets;
    int height,width;
    int lives;
    void intro();
public:
    bool    wannaContinue;
            Level(int l, int s, int lc, int w, int h );
            ~Level();
    int     getScore(){ return score; }
    void    play();
    void    scoreIncrease();
    void    moveBullets();
    void    died();
    void    obstacleCreator();
    void    hitByMyBullet();
    void    ask();
    bool    obstacleHitted( const pair<int,int> & c);
    bool    hitByEnemyBullet( const pair<int,int> & c );
    void    writePlayer();
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

Level::Level(int l, int s, int lc, int w, int h ):level(l),wannaContinue(true), myBullet(nullptr), height(h), width(w), lives(lc), score(s)
{
    initscr();
    win = newwin(height,width,0,0);
    refresh();
    box(win,0,0);
    intro();
    box(win,0,0);

    enemyArmy = new EnemyArmy(win, 3, width);
    spaceShip = new SpaceShip(width,height,win);
    obstacleCreator();
    mvwprintw(win,2,2,"SCORE: %d", score);
    mvwprintw(win,height-1,2,"LIVES: %d", lives);
    wrefresh(win);
    refresh();
}


Level::~Level(){
    delete myBullet;
    delete spaceShip;
    delete enemyArmy;
    enemyBullets.clear();
    int pocet = obstacles.size();
    for ( int i = 0; i < pocet ; i++)
        delete obstacles[i];
}


void Level::obstacleCreator(){
    for ( int i = 5; i < width -10; i+= 15 )
        obstacles.push_back(new Obstacle(win,i,height-10) );
}

vector<pair<string,int>> getPlayers(int score){
    ifstream file;
    file.open("top_five.txt");
    string nickname;
    int best_score;
    vector<pair<string, int>> topky;
    while ( file >> nickname && file >> best_score )
        topky.emplace_back( nickname, best_score );
    pair<string, int> newone("",score);
    sort(topky.begin(), topky.end(), [](const pair<string,int>&a, const pair<string,int>&b )
    { return a.second > b.second; } );
    auto i = lower_bound(topky.begin(), topky.end(),newone, [](const pair<string,int>&a, const pair<string,int>&b )
    { return a.second > b.second; } );

    topky.insert(i, newone);
    file.close();
    return topky;
}

void writeToFile( vector<pair<string,int>> & topky ){
    ofstream file;
    file.open("top_five.txt");
    for ( int j = 0; j < topky.size() and j < 5 ; j++ ){
        file << topky[j].first << " " << topky[j].second << endl;
    }

    file.close();
}


void Level::writePlayer() {
    nodelay(stdscr, FALSE);
    echo();
    vector<pair<string, int>> topky = getPlayers(score);
    int index = -1;
    pair<int,int> edit(-1,-1);
    for ( int j = 0; j < topky.size() and j < 5 ; j++ ){
        mvwprintw(win,height/2+j,width/2-5,"%d    %s",topky[j].second, topky[j].first.c_str() );
        if ( topky[j].second == score and topky[j].first == "" ){
            edit = make_pair(width/2-1+int(log10(score) + 1),height/2+j );
            index = j;
        }
    }
    if ( index == 4 ){
        noecho();
        wclear( win );
        wrefresh(win);
        return;
    }

    move(edit.second,edit.first);
    wrefresh(win);

    char a ;
    string playerNick;
    do{
        a = getch();
        playerNick += a;
    }while( a != '\n' or playerNick.size()==1 );
    playerNick.pop_back();
    topky[index].first = playerNick;
    writeToFile(topky);
    noecho();
    wclear( win );
    wrefresh(win);

}


void Level::ask(){
    wclear(win);
    box(win,0,0);
    mvwprintw( win, height/2-5, width/2-4,"GAME OVER");
    writePlayer();
    mvwprintw( win, height/2 + 2, width/2-15,"If you want continue press 'Y'");
    nodelay(stdscr, FALSE);
    wrefresh(win);
    char a = getch();
    if ( a != 'y' )
        wannaContinue = false;
    wclear(win);
    wrefresh(win);
}


void Level::died(){
    sleep(2);
    delete spaceShip;
    spaceShip = nullptr;
    if ( ! --lives )
        return;

    spaceShip = new SpaceShip(width,height,win);
    spaceShip->showMe();
    mvwprintw(win,height-1,2,"LIVES: %d", lives);
    enemyBullets.clear();
    wrefresh(win);
}

bool    Level::obstacleHitted( const pair<int,int> & c){

    for ( int i = 0; i < obstacles.size() ; i++ )
        if ( obstacles[i]->contains(c) ){
            if ( obstacles[i]->getDamage(c) ){
                delete obstacles[i];
                obstacles.erase(obstacles.begin()+i);
            }
            return false;
        }
    return true;
}


void Level::scoreIncrease() {
    score += 50;
    mvwprintw(win,2,2,"SCORE: %d", score);
    wrefresh(win);
}



void Level::hitByMyBullet(){

    pair<int,int> colision = make_pair( myBullet->getCoords().first,myBullet->getCoords().second-1 );

    if ( obstacleHitted( colision ) )
        scoreIncrease();
    enemyArmy->killEnemy( colision );

    delete myBullet;
    myBullet = nullptr;
}

bool Level::hitByEnemyBullet( const pair<int,int> & c ){
    if ( spaceShip->contains(make_pair(c.first,c.second+1)) )
        return true;
    obstacleHitted(make_pair(c.first,c.second+1));
    return false;
}


void Level::moveBullets() {
    if ( myBullet && ! myBullet->move() )
        hitByMyBullet();

    for ( auto enemyBullet = enemyBullets.begin(); enemyBullet != enemyBullets.end(); enemyBullet++ )        //----------------nedokonane------------------------------
        if ( !(*enemyBullet)->move() ){
            if ( hitByEnemyBullet((*enemyBullet)->getCoords() ) )
                died();
            else
                enemyBullets.erase(enemyBullet);
            return;
        }
}


void Level::play(){

    noecho();
    nodelay(stdscr, TRUE);

    int shoot = 40;

    while ( enemyArmy->isAlive() and spaceShip != nullptr ){
        char a = 'q' ;
        if ( ( a = getch() ) != ERR ){
            switch (a) {
                case 'a': spaceShip->moveLeft(); break;
                case 'd': spaceShip->moveRight(); break;
                case ' ':{
                    if ( myBullet == nullptr )
                        myBullet = spaceShip->shoot();
                } break;
                default: break;
            }
        }

        if ( ! shoot-- ){
            enemyBullets.emplace_back(enemyArmy->enemyFire());
            shoot = 40;
        }

        enemyArmy->moveArmy();
        moveBullets();

        usleep(100000);
    }
    if ( spaceShip == nullptr )
        ask();

}



//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
int main(){
    bool wantContinue = true;
    int i = 0;
    int score = 0;
    int livesCount = 1;
    int width = 65;
    int height = 30;

    while ( wantContinue ){
        Level * lvl = new Level(++i, score, livesCount, width, height);
        lvl->play();


        wantContinue = lvl->wannaContinue;
        endwin();
        score = lvl->getScore();
        delete lvl;
    }

    return 0;
}
