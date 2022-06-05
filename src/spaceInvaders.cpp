#include <ncurses.h>
#include <iostream>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include <vector>
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
    auto i = find(coords.begin(), coords.end(), c);
    if ( i != coords.end() )
        return true;
    return false;
}

void Object::showMe(){
    for ( const auto & i: coords )
        mvwprintw(win,i.second,i.first,me);
    wrefresh(win);

}


//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
class Bullet: public Object{
private:
    bool direction;
public:
                    Bullet(WINDOW *w, int a, int b, bool d);
                    ~Bullet();
    pair<int,int>   getCoords(){ return coords[0]; }
    bool            canMove(int x, int y) override;
    bool            move();
};

Bullet::Bullet(WINDOW *w, int a, int b, bool d) : Object(w,"|"), direction(d) {
    coords.emplace_back(a,b);
    showMe();
}

Bullet::~Bullet(){
    mvwprintw( win, coords[0].second, coords[0].first, " " );
    wrefresh(win);
}


bool Bullet::canMove(int x, int y) {
    auto  daco = chtype mvwinch( win, y, x);
    if (  daco == 'X' && ! direction  )   // no hitting enemy by enemy
        return true;
    return daco == ' ';
}

bool Bullet::move(){
    int old_y = coords[0].second;
    if ( direction && canMove( coords[0].first, --coords[0].second ) ){
        showMe();
        mvwprintw(win,coords[0].second+1,coords[0].first," ");
        wrefresh(win);
        return true;
    }
    else if ( canMove( coords[0].first, ++coords[0].second ) ){
        showMe();
        mvwprintw(win,coords[0].second-1,coords[0].first," ");
        wrefresh(win);
        return true;
    }
    coords[0].second = old_y;
    return false;
}


//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

class Obstacle: public Object{
public:
            Obstacle(WINDOW * w, int a, int b);
            Obstacle(WINDOW * w, vector<pair<int,int>> v);
    bool    getDamage( const pair<int,int> & c);
    void    writeCoords( ofstream & file);
};
Obstacle::Obstacle(WINDOW *w, vector<pair<int, int>> v): Object(w,"Q") {
    coords = std::move(v);
    showMe();
}

Obstacle::Obstacle(WINDOW *w, int a, int b): Object(w,"Q") {
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
    coords.erase(find(coords.begin(), coords.end(), c) );
    mvwprintw(win,c.second,c.first," ");
    wrefresh(win);
    return coords.empty();
}

void Obstacle::writeCoords(ofstream &file) {
    for ( const auto & i : coords )
        file << i.first << " " << i.second << endl;
    file << endl;
}


//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

class Enemy:public Object{
public:
                        Enemy(WINDOW * w, int a, int b);
                        ~Enemy();
    void                moveLeft();
    void                moveRight();
    void                getDown();
    void                move( bool direction ) override;
    unique_ptr<Bullet>  shoot() const;
};



Enemy::Enemy(WINDOW * w, int a, int b): Object(w,"X") {
    x = a; y = b;
    coords.emplace_back(x,y);
    coords.emplace_back(x+1,y);
    coords.emplace_back(x+2,y);
    coords.emplace_back(x,y+1);
    coords.emplace_back(x+2,y+1);
    showMe();
}

Enemy::~Enemy() {
    for (const auto &i: coords)
        mvwprintw(win, i.second, i.first, " ");
}

unique_ptr<Bullet>  Enemy::shoot() const {
    return make_unique<Bullet> (Bullet(win,x+1,y+2,false));
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
    int downEdge;   // donw edge of army
    int bounceCount;
    int width;
public:
    int downLimit;  // donw limit influenced by obstacles and spaceship

                        EnemyArmy( WINDOW * win, int count , int _width, int _limit );
                        ~EnemyArmy();
    bool                isAlive(){ return !enemies.empty(); }
    void                moveArmy();
    bool                killEnemy( pair<int,int> coords );
    void                getCloser();
    void                checkChange();
    unique_ptr<Bullet>  enemyFire() const ;
};

EnemyArmy::EnemyArmy(WINDOW *win, int count, int _width, int _limit):bounceCount(0),direction(true),width(_width),downLimit(_limit){
    for ( int i = 1 ; i < count+1 ; i++ )
        for ( int j = 0 ; j < 12 ; j+=3 )
            enemies.emplace_back(make_unique<Enemy>( win,i*5 + 10, j + 5 ));
    downEdge = 18;  // limit for one row is 6, add 4 for every next row
    leftEdge = 15;
    rightEdge = 15 + count * 5;
    wrefresh( win );
}

EnemyArmy::~EnemyArmy(){
    enemies.clear();
}

unique_ptr<Bullet> EnemyArmy::enemyFire() const {
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
    if ( bounceCount == 1 && downEdge < downLimit ){
        getCloser();
        bounceCount = 0;
    }

}

void EnemyArmy::getCloser() {
    downEdge++;
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


bool EnemyArmy::killEnemy( pair<int,int> coords ) {
    for ( auto i = enemies.begin() ; i != enemies.end() ; i++ ){
        if ( (*i)->contains(coords ) ){
            enemies.erase(i);
            return true;
        }
    }
    return false;
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
class SpaceShip : public Object {
private:
    int width;
public:
                        SpaceShip( int a, int b, WINDOW * w );
                        ~SpaceShip();
    bool                canMove( int x, int y ) override;
    void                moveLeft();
    void                moveRight();
    Bullet *            shoot();
};



SpaceShip::SpaceShip( int a, int b, WINDOW * w ): Object(w,"O"),width(a){
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

SpaceShip::~SpaceShip(){
    for ( const auto & i : coords )
        mvwprintw(win,i.second,i.first," ");
}


bool SpaceShip::canMove( int x, int y ){
    if (  x <= 0 || x + 9 >= width )
        return true;
    return false;
}

Bullet* SpaceShip::shoot(){
    return new Bullet(win,x+4,y-3,true);
}

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
    SpaceShip * spaceShip;
    Bullet  * myBullet;
    EnemyArmy * enemyArmy;
    WINDOW  * win;
    list<unique_ptr<Obstacle>> obstacles;
    list<unique_ptr<Bullet>> enemyBullets;
    int height,width;
    string  getNickName( const pair<int,int>& position );
    void    intro();
    void    scoreIncrease();
    void    moveBullets();
    void    playerDied();
    void    obstacleCreator();
    void    hitByMyBullet();
    void    ask();
    bool    obstacleHitted( const pair<int,int> & c);
    bool    hitByEnemyBullet( const pair<int,int> & c );
    void    bulletHitted( const pair<int,int> & c);
    void    writePlayer();
    void    saveObstacles();
    void    obstacleReader();
public:
    int level;
    int score;
    int lives;
    bool fromStart;
    bool    wannaContinue;
            Level(int _lvl, int _score, int _lives, int _width, int _height );
            ~Level();
    void    play();

};


Level::Level(int _lvl, int _score, int _lives, int _width, int _height ): level(_lvl), wannaContinue(true), myBullet(nullptr), height(_height), width(_width), lives(_lives), score(_score), fromStart(false)
{
    win = newwin(height,width,0,0);
    intro();
    box(win,0,0);

    enemyArmy = new EnemyArmy(win, 8, width,height-12);
    spaceShip = new SpaceShip(width,height,win);

    if ( level == 1 )
        obstacleCreator();
    else
        obstacleReader();

    mvwprintw(win,2,2,"SCORE: %d", score);
    mvwprintw(win,height-1,2,"LIVES: %d", lives);
    wrefresh(win);
}


Level::~Level(){
    delete myBullet;
    delete spaceShip;
    delete enemyArmy;
    enemyBullets.clear();
    obstacles.clear();
    delwin(win);
}


void Level::obstacleCreator(){
    for ( int i = 5; i < width -10; i+= 15 )
        obstacles.emplace_back(make_unique<Obstacle> (Obstacle(win,i,height-10) ));
}


void Level::intro() {
    ifstream file;
    file.open("top_five.txt");
    string nickname;
    int i = 0;
    mvwprintw(win,height/2-4,width/2-7,"SPACE INVADERS");
    mvwprintw(win,height/2-2,width/2-7,"top 5 players");

    while ( getline(file, nickname)  )
        mvwprintw(win,height/2+i++,width/2-6,nickname.c_str());

    mvwprintw(win,height/2+10,width/2-12,"(press any key to start)");
    move(0,0);
    box(win,0,0);
    wrefresh(win);
    getch();
    wclear(win);
    file.close();
}

void myCleaner(){
    nodelay(stdscr, TRUE);
    char m;
    while ( (m = getch()) != ERR )
        continue;
    nodelay(stdscr, FALSE);
}

void Level::obstacleReader() {
    ifstream file;
    file.open("obstacles.txt");
    pair<int,int> obstacleCoords;

    while ( ! file.eof() ){
        vector<pair<int,int>> allCords;
        while ( file >> obstacleCoords.first and file >> obstacleCoords.second )
            allCords.push_back(obstacleCoords);
        obstacles.emplace_back(make_unique<Obstacle>( Obstacle(win,allCords)));
    }

    file.close();

}

vector<pair<string,int>> getPlayers(int score){
    ifstream file;
    file.open("top_five.txt");
    string nickname;
    int best_score;
    vector<pair<string, int>> topPlayers;
    while ( file >> nickname && file >> best_score )
        topPlayers.emplace_back( nickname, best_score );

    pair<string, int> newone("",score);

    sort(topPlayers.begin(), topPlayers.end(),
         [](const pair<string,int>&a, const pair<string,int>&b )
                    { return a.second > b.second; } );

    auto i = lower_bound(topPlayers.begin(), topPlayers.end(), newone,
                         [](const pair<string,int>&a, const pair<string,int>&b )
                                { return a.second > b.second; } );

    topPlayers.insert(i, newone);
    file.close();
    return topPlayers;
}


void writeToFile( vector<pair<string,int>> & topPlayers ){
    ofstream file;
    file.open("top_five.txt");
    for (int j = 0; j < topPlayers.size() and j < 5 ; j++ )
        file << topPlayers[j].first << " " << topPlayers[j].second << endl;
    file.close();
}




string Level::getNickName( const pair<int,int>& position ){
    mvwprintw(win,height/2-2,width/2-19,"Enter your nickname(10 characters max)");
    move(position.second, position.first);
    wrefresh(win);
    myCleaner();
    echo();
    char a ;
    string playerNick;
    do{
        a = getch();
        playerNick += a;
    } while( ( a != '\n' or playerNick.size()==1) and playerNick.size()<=10 );
    playerNick.pop_back();
    noecho();
    return playerNick;
}

void Level::writePlayer() {
    vector<pair<string, int>> topPlayers = getPlayers(score);
    int index = -1;
    pair<int,int> position(-1, -1); //position of

    for (int j = 0; j < topPlayers.size() and j < 5 ; j++ ){
        mvwprintw(win,height/2+j,width/2-5, "%d    %s", topPlayers[j].second, topPlayers[j].first.c_str() );
        if (topPlayers[j].second == score and topPlayers[j].first.empty() ){
            position = make_pair(width / 2 - 1 + int(log10(score) + 1), height / 2 + j );
            index = j;
        }
    }

    if ( index != -1 ){
        topPlayers[index].first = getNickName(position);
        writeToFile(topPlayers);
    }

    wclear( win );
    box(win,0,0);
    wrefresh(win);
}


void Level::ask(){
    wclear(win);
    box(win,0,0);
    mvwprintw( win, height/2-5, width/2-4,"GAME OVER");
    if ( score > 0 )
        writePlayer();
    mvwprintw( win, height/2 , width/2-12,"press 'Y' to play again");
    myCleaner();

    wrefresh(win);
    if ( getch() != 'y' )
        wannaContinue = false;
    else
        fromStart = true;
    wclear(win);
    wrefresh(win);
}


void Level::playerDied(){
    sleep(2);
    delete spaceShip;
    spaceShip = nullptr;
    if ( ! --lives )
        return;

    spaceShip = new SpaceShip(width,height,win);
    spaceShip->showMe();
    mvwprintw(win,height-1,2,"LIVES: %d", lives);
    enemyBullets.clear();
    delete myBullet;
    myBullet = nullptr;
    wrefresh(win);
}


bool    Level::obstacleHitted( const pair<int,int> & c){

    for ( auto i = obstacles.begin() ; i !=  obstacles.end() ; i++ )
        if ( (*i)->contains(c) ){
            if ( (*i)->getDamage(c) )
                obstacles.erase(i);
            return false;
        }
    return true;
}


void Level::scoreIncrease() {
    score += 45 + level * 5 ;
    mvwprintw(win,2,2,"SCORE: %d", score);
    wrefresh(win);
}

void Level::bulletHitted(const pair<int,int> & c){
    for ( auto i = enemyBullets.begin(); i != enemyBullets.end(); i++ )
        if ( (*i)->contains(c) ){
            enemyBullets.erase(i);
            return;
        }
}


void Level::hitByMyBullet(){

    pair<int,int> colision = make_pair( myBullet->getCoords().first,myBullet->getCoords().second-1 );

    obstacleHitted(colision);
    bulletHitted(colision);
    if ( enemyArmy->killEnemy( colision ))
        scoreIncrease();

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

    for ( auto enemyBullet = enemyBullets.begin(); enemyBullet != enemyBullets.end(); enemyBullet++ )
        if ( !(*enemyBullet)->move() ){
            if ( hitByEnemyBullet((*enemyBullet)->getCoords() ) )
                playerDied();
            else
                enemyBullets.erase(enemyBullet);
            return;
        }
}


void Level::saveObstacles(){
    ofstream file;
    file.open("obstacles.txt");

    for ( const auto & i : obstacles )
        i->writeCoords(file);

    file.close();
}


void Level::play(){

    noecho();
    nodelay(stdscr, TRUE);
    int shootFrequency = 45 - level * 5 ;

    while ( enemyArmy->isAlive() and spaceShip != nullptr ){
        char a;
        if ( ( a = getch() ) != ERR ){
            switch (a) {
                case 'a': spaceShip->moveLeft(); break;
                case 'd': spaceShip->moveRight(); break;
                case ' ':
                {
                    if ( myBullet == nullptr )
                        myBullet = spaceShip->shoot();
                } break;
                default: break;
            }
        }

        if ( ! shootFrequency-- ){
            enemyBullets.emplace_back(enemyArmy->enemyFire());
            shootFrequency = 40 - level*5 ;
        }
        enemyArmy->moveArmy();
        moveBullets();
        usleep(100000);
    }
    if ( spaceShip == nullptr )
        ask();
    else{
        saveObstacles();
        score += 100;
    }

}



//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
int main(){


    bool wantContinue = true;
    int level = 0;
    int score = 0;
    int livesCount = 3;
    int width = 95;
    int height = 34;
    initscr();
    refresh();
    while ( wantContinue ){
        Level * lvl = new Level(++level, score, livesCount, width, height);
        lvl->play();
        wantContinue = lvl->wannaContinue;
        if ( lvl->fromStart ){
            score = 0;
            livesCount = 3;
            level = 0;
        }
        else{
            score = lvl->score;
            livesCount = lvl->lives;
            level = lvl->level;
        }
        delete lvl;
    }
    endwin();
    return 0;
}
