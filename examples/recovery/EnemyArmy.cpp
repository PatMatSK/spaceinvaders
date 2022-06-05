#include "EnemyArmy.h"

using namespace std;

EnemyArmy::EnemyArmy(WINDOW *win, int count, int _width, int _limit):
    bounceCount(0),direction(true),width(_width),downLimit(_limit),ableToMove(true)
{
    for ( int i = 1 ; i < count+1 ; i++ )
        for ( int j = 0 ; j < 6 ; j+=3 )
            enemies.emplace_back(make_unique<Enemy>( win,i*5 + 10, j + 3 ));
    downEdge = 9;  // limit for one row is 5, add 4 for every next row
    leftEdge = 15;
    rightEdge = 15 + count * 5;
    wrefresh( win );
}

EnemyArmy::~EnemyArmy()
{
    enemies.clear();
}

unique_ptr<Bullet> EnemyArmy::enemyFire() const
{
    auto it = enemies.cbegin();
    int random = rand() % enemies.size();
    advance(it, random);
    return (*it)->shoot();
}

/*
 * Decides whether to check direction of enemies.
 */
void EnemyArmy::checkChange()
{
    if ( rightEdge >= width )
    {
        direction = false;
        bounceCount++;
    }
    if ( leftEdge <= 1 )
    {
        direction = true;
        bounceCount++;
    }
    if ( bounceCount == 1 && downEdge < downLimit )
    {
        getCloser();
        bounceCount = 0;
    }

}

void EnemyArmy::getCloser()
{
    downEdge++;
    for ( auto & i : enemies )
        i->getDown();
}

void EnemyArmy::moveArmy()
{
    if ( ! ableToMove )
        return;

    checkChange();
    for ( auto & i : enemies )
        i->move(direction);

    if ( direction )
    {
        leftEdge++;
        rightEdge++;
    }
    else
    {
        leftEdge--;
        rightEdge--;
    }
}

/*
 * Destroys enemy on given position.
 */
bool EnemyArmy::killEnemy( pair<int,int> coords )
{
    for ( auto i = enemies.begin() ; i != enemies.end() ; i++ )
    {
        if ( (*i)->contains(coords ) )
        {
            enemies.erase(i);
            return true;
        }
    }
    return false;
}
