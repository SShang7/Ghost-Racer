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
    m_gr = new GhostRacer();
    aloa.push_back(m_gr);
    int N = VIEW_HEIGHT / SPRITE_HEIGHT;
    int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH / 2;
    int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH / 2;
    for(int i = 0; i < N; ++i){
        int new_y = i * SPRITE_HEIGHT;
        Actor* left = new Border(true, LEFT_EDGE, new_y);
        aloa.push_back(left);
        Actor* right = new Border(true, RIGHT_EDGE, new_y);
        aloa.push_back(right);
    }
    int M = VIEW_HEIGHT / (4 * SPRITE_HEIGHT);
    for (int i = 0; i < M; i++) {
        Actor* left = new Border(false, LEFT_EDGE + ROAD_WIDTH / 3, i * (4 * SPRITE_HEIGHT));
        aloa.push_back(left);
        Actor* right = new Border(false, RIGHT_EDGE + ROAD_WIDTH / 3, i * (4 * SPRITE_HEIGHT));
        aloa.push_back(right);
    }

    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    decLives();
    return GWSTATUS_PLAYER_DIED;
}

void StudentWorld::cleanUp()
{
}

StudentWorld::~StudentWorld() {
    cleanUp();
}