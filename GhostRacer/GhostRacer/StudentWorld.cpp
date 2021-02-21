#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

int StudentWorld::init()
{
    m_gr = new GhostRacer(this);
    aloa.push_back(m_gr);
    int N = VIEW_HEIGHT / SPRITE_HEIGHT;
    for(int i = 0; i < N; ++i){
        int new_y = i * SPRITE_HEIGHT;
        Actor* left = new Border(this, true, LEFT_EDGE, new_y);
        aloa.push_back(left);
        Actor* right = new Border(this, true, RIGHT_EDGE, new_y);
        aloa.push_back(right);
    }
    int M = VIEW_HEIGHT / (4 * SPRITE_HEIGHT);
    for (int i = 0; i < M; i++) {
        Actor* left = new Border(this, false, LEFT_EDGE + ROAD_WIDTH / 3, i * (4 * SPRITE_HEIGHT));
        aloa.push_back(left);
        Actor* right = new Border(this, false, RIGHT_EDGE - ROAD_WIDTH / 3, i * (4 * SPRITE_HEIGHT));
        aloa.push_back(right);
    }
    last_WB_y = aloa.back()->getY();
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    for (list<Actor*>::iterator i = aloa.begin(); i != aloa.end(); ++i) {
        if ((*i)->getLiving()) {
            (*i)->doSomething();
            
            if (!m_gr->getLiving()) {
                decLives();
                return GWSTATUS_PLAYER_DIED;
            }
        }
        
    }
    //remove dead objects
    for (list<Actor*>::iterator i = aloa.begin(); i != aloa.end();) {
        if (!(*i)->getLiving()) {
            delete* i;
            i = aloa.erase(i);
        }
        else ++i;
    }
    last_WB_y += -4 - m_gr->getSpeedV();
    int new_border_y = VIEW_HEIGHT - SPRITE_HEIGHT;
    int delta_y = new_border_y - last_WB_y;
    if (delta_y >= SPRITE_HEIGHT) {
        Actor* left = new Border(this, true, LEFT_EDGE, new_border_y);
        aloa.push_back(left);
        Actor* right = new Border(this, true, RIGHT_EDGE, new_border_y);
        aloa.push_back(right);
    }
    if (delta_y >= 4 * SPRITE_HEIGHT) {
        Actor* left = new Border(this, false, LEFT_EDGE + ROAD_WIDTH / 3, new_border_y);
        aloa.push_back(left);
        Actor* right = new Border(this, false, RIGHT_EDGE - ROAD_WIDTH / 3, new_border_y);
        aloa.push_back(right);
        last_WB_y = aloa.back()->getY();
    }
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for (list<Actor*>::iterator i = aloa.begin(); i != aloa.end();) {
        delete* i;
        i = aloa.erase(i);
    }
}

StudentWorld::~StudentWorld() {
    cleanUp();
}

GhostRacer* StudentWorld::getGR() {
    return m_gr;
}