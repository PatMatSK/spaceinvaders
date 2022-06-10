#include "Level.h"
#define BONUS_COUNT 4

using namespace std;

Level::Level(int _lvl, int _score, int _lives, int _width, int _height ): myBullet(nullptr), height(_height), width(_width),
                                            level(_lvl), score(_score), lives(_lives), fromStart(false), wannaContinue(true)
{
    win = newwin(height,width,0,0);
    intro();
    box();
    loadBonuses();
    enemyArmy = new EnemyArmy(win, 3, width,height-12);
    spaceShip = new SpaceShip(width,height,win);

    if ( level == 1 )
        obstacleCreator();
    else
        obstacleReader();

    mvwprintw(win,2,2,"SCORE: %d", score);
    mvwprintw(win,height-1,2,"LIVES: %d", lives);
    wrefresh(win);
}

Level::~Level()
{
    delete myBullet;
    delete spaceShip;
    delete enemyArmy;
    enemyBullets.clear();
    obstacles.clear();
    bonuses.clear();
    delwin(win);
}

void Level::obstacleCreator()
{
    for ( int i = 5; i < width -10; i+= 15 )
        obstacles.emplace_back(make_unique<Obstacle> (Obstacle(win,i,height-10) ));
}

/*
 * Shows introduction with current top players.
 */
void Level::intro()
{
    ifstream file;
    file.open("assets/top_five.txt");
    string nickname;
    int i = 0;
    mvwprintw(win,height/2-4,width/2-7,"SPACE INVADERS");
    mvwprintw(win,height/2-2,width/2-7,"top 5 players");

    while ( getline(file, nickname)  )
        mvwprintw(win,height/2+i++,width/2-6,nickname.c_str());

    mvwprintw(win,height/2+10,width/2-12,"(press any key to start)");
    move(0,0);
    box();
    wrefresh(win);
    getch();
    wclear(win);
    file.close();
}

void Level::box()
{
    for ( int i = 0; i < height ; i++ )
    {
        mvwprintw(win,i,0,"|");
        mvwprintw(win,i,width-1,"|");
    }
    for ( int i = 0; i < width ; i++ )
    {
        mvwprintw(win,0,i,"-");
        mvwprintw(win,height-1,i,"-");
    }
    mvwprintw(win,0,0,"+");
    mvwprintw(win,0,width-1,"+");
    mvwprintw(win,height-1,0,"+");
    mvwprintw(win,height-1,width-1,"+");
}

void Level::loadBonuses()
{
    bonuses.emplace_back(make_unique<BonusRepair> (BonusRepair()));
    bonuses.emplace_back(make_unique<BonusStop> (BonusStop()));
    bonuses.emplace_back(make_unique<BonusDeleteObstacle> (BonusDeleteObstacle()));
    bonuses.emplace_back(make_unique<BonusShield> (BonusShield()));
}

void myCleaner()
{
    nodelay(stdscr, TRUE);
    char m;
    while ( (m = getch()) != ERR )
        continue;
    nodelay(stdscr, FALSE);
}

void Level::obstacleReader()
{
    ifstream file;
    file.open("assets/obstacles.txt");
    pair<int,int> obstacleCoords;

    while ( ! file.eof() )
    {
        vector<pair<int,int>> allCords;
        while ( file >> obstacleCoords.first and file >> obstacleCoords.second )
            allCords.push_back(obstacleCoords);
        obstacles.emplace_back(make_unique<Obstacle>( Obstacle(win,allCords)));
    }
    file.close();
}

/*
 * Gets all current best players and add actual player. Then returns sorted by score.
 */
vector<pair<string,int>> getPlayers(int score)
{
    ifstream file;
    file.open("assets/top_five.txt");
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

void writeToFile( vector<pair<string,int>> & topPlayers )
{
    ofstream file;
    file.open("assets/top_five.txt");
    for (int j = 0; j < topPlayers.size() and j < 5 ; j++ )
        file << topPlayers[j].first << " " << topPlayers[j].second << endl;
    file.close();
}

string Level::getNickName( const pair<int,int>& position )
{
    mvwprintw(win,height/2-2,width/2-19,"Enter your nickname(10 characters max)");
    move(position.second, position.first);
    wrefresh(win);
    myCleaner();
    echo();
    char a ;
    string playerNick;
    do
    {
        a = getch();
        playerNick += a;
    } while( ( a != '\n' or playerNick.size()==1) and playerNick.size()<=10 );
    playerNick.pop_back();
    noecho();
    return playerNick;
}

/*
 * Firstly gets sorted top scores included actual score and if player belongs to top 5, rewrites file.
 * Variable @position is used to move cursor on correct position while writing his nickname.
 */
void Level::writePlayer()
{
    vector<pair<string, int>> topPlayers = getPlayers(score);
    int index = -1;
    pair<int,int> position(-1, -1);

    for (int j = 0; j < topPlayers.size() and j < 5 ; j++ )
    {
        mvwprintw(win,height/2+j,width/2-5, "%d    %s", topPlayers[j].second, topPlayers[j].first.c_str() );
        if (topPlayers[j].second == score and topPlayers[j].first.empty() )
        {
            position = make_pair(width / 2 - 1 + int(log10(score) + 1), height / 2 + j );
            index = j;
        }
    }

    if ( index != -1 )
    {
        topPlayers[index].first = getNickName(position);
        writeToFile(topPlayers);
    }

    wclear( win );
    box();
    wrefresh(win);
}

/*
 * Call function to decide whether player is worth to write to table
 * and asks player to play again or quit.
 */
void Level::ask()
{
    wclear(win);
    box();
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

/*
 * Player died, so game continues with decreased lives count and new spaceship
 * or spaceship is dead and level is over.
 */
void Level::playerDied()
{
    if ( spaceShip->shield )
    {
        spaceShip->deactivateShield();
        enemyBullets.clear();
        return;
    }
    sleep(2);
    myCleaner();
    nodelay(stdscr, TRUE);
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


bool    Level::obstacleHitted( const pair<int,int> & c)
{
    for ( auto i = obstacles.begin() ; i !=  obstacles.end() ; i++ )
        if ( (*i)->contains(c) )
        {
            if ( (*i)->getDamage(c) )
                obstacles.erase(i);
            return false;
        }
    return true;
}


void Level::scoreIncrease()
{
    score += 45 + level * 5 ;
    mvwprintw(win,2,2,"SCORE: %d", score);
    wrefresh(win);
}

void Level::bulletHitted(const pair<int,int> & c)
{
    for ( auto i = enemyBullets.begin(); i != enemyBullets.end(); i++ )
        if ( (*i)->contains(c) )
        {
            enemyBullets.erase(i);
            return;
        }
}

void Level::makeBonus()
{
    bonuses[0]->applyBonus(spaceShip,enemyArmy,obstacles);
    /*
    int i = rand() % 100;
    if ( i < 100 )
        bonuses[ rand() % BONUS_COUNT ]->applyBonus(spaceShip,enemyArmy,obstacles);
    */
}

/*
 * Detect whitch object is hitted and if hitted enemy -> increase score, destroy bullet.
 */
void Level::hitByMyBullet()
{
    pair<int,int> colision = make_pair( myBullet->getCoords().first,myBullet->getCoords().second-1 );

    obstacleHitted(colision);
    bulletHitted(colision);
    if ( enemyArmy->killEnemy( colision ))
    {
        scoreIncrease();
        makeBonus();
    }

    delete myBullet;
    myBullet = nullptr;
}


bool Level::hitByEnemyBullet( const pair<int,int> & c )
{
    if ( spaceShip->contains(make_pair(c.first,c.second+1)) )
        return true;
    obstacleHitted(make_pair(c.first,c.second+1));
    return false;
}


void Level::moveBullets()
{
    if ( myBullet && ! myBullet->move(true) )
        hitByMyBullet();

    for ( auto enemyBullet = enemyBullets.begin(); enemyBullet != enemyBullets.end(); enemyBullet++ )
        if ( !(*enemyBullet)->move(false) )
        {
            if ( hitByEnemyBullet((*enemyBullet)->getCoords() ) )
                playerDied();
            else
                enemyBullets.erase(enemyBullet);
            return;
        }
}

/*
 * Store obstacles in current state to next level.
 */
void Level::saveObstacles()
{
    ofstream file;
    file.open("assets/obstacles.txt");

    for ( const auto & i : obstacles )
        i->writeCoords(file);

    file.close();
}

void Level::moveSpaceship()
{
    char a;
    if ( ( a = getch() ) != ERR )
    {
        switch (a) {
            case 'a': spaceShip->move(false); break;
            case 'd': spaceShip->move(true); break;
            case ' ':
            {
                if ( myBullet == nullptr )
                    myBullet = spaceShip->shoot();
            } break;
            default: break;
        }
    }
}

/*
 * This is main game loop where are all moving objects getting moved, and enemies shoot if they can.
 * Level continues if there are still some enemies and we have our spaceship.
 * When Level is over, saves the obstacles for next level (if player won) or ask player whether to play again.
 */
void Level::play()
{
    noecho();
    nodelay(stdscr, TRUE);
    int shootFrequency = max( 10 - level * 5, 10 ) ;
    int stopArmy = 0;
    while ( enemyArmy->isAlive() and spaceShip != nullptr )
    {
        moveSpaceship();
        //enemyArmy->moveArmy();
        moveBullets();
/*
        if ( ! shootFrequency-- )
        {
            enemyBullets.emplace_back(enemyArmy->enemyFire());
            shootFrequency = max( 45 - level * 5, 10 ) ;
        }
        if ( stopArmy == 1 )
        {
            enemyArmy->ableToMove = true;
            stopArmy = 0;
        }

        if ( ! enemyArmy->ableToMove and --stopArmy == -1 )
            stopArmy = 50;
*/
        usleep(100000);
    }
    if ( spaceShip == nullptr )
        ask();
    else
    {
        saveObstacles();
        score += 100;
    }

}

