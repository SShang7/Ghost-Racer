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
    StudentWorld(std::string assetPath); //constructor
    ~StudentWorld(); //destructor
    virtual int init(); //initializes world
    virtual int move(); //updates each tick
    virtual void cleanUp(); //removes all the allocated memory
    GhostRacer* getGR() const; //gets point to Ghost Racer
    void addActor(Actor* a);
    void addSoul();
    bool overlap(Actor* a1, Actor* a2) const;
    bool overlapWater(Actor* a);
    Actor* closestFrontCollLane(Actor* a);
    Actor* closestBehindCollLane(Actor* a);
    // If actor a overlaps this world's GhostRacer, return a pointer to the
    // GhostRacer; otherwise, return nullptr
    GhostRacer* getOverlappingGhostRacer(Actor* a) const;
private:
    std::list<Actor*> aloa;
    GhostRacer* m_gr;
    int last_WB_y;
    int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH / 2;
    int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH / 2;
    int m_soulCount;
    int m_bonus;
    int lane(double x);
    Actor* closestBotCollLane(int l);
    Actor* closestTopCollLane(int l);
    
};

#endif // STUDENTWORLD_H_
