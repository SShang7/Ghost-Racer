#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <string>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;
class GhostRacer;
class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    GhostRacer* getGR();
private:
    std::list<Actor*> aloa;
    GhostRacer* m_gr;
    int last_WB_y;
    int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH / 2;
    int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH / 2;
};

#endif // STUDENTWORLD_H_
