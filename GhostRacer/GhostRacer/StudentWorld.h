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
    GhostRacer* getGR() const; //gets point to Ghost Racer. Allows other classes to modify and access GhostRacer.
    void addActor(Actor* a); //adds an Actor the the end of the list of actors. Allows for creation of Spray, HealingGoodie, and OilSlick.
    void addSoul(); //increments m_soul, the soul count. Specifically used for collection of SoulGoodie.
    bool overlapWater(Actor* a); //checks if an actor is overlapping with spray projectile. Useful for dealing with collisions between Spray and other Actors.
    Actor* closestFrontCollLane(Actor* a); //finds the closest collision-avoidance worth actor in front of a ZombieCab within 96 pixels. Returns nullptr if none found. Useful for ZombieCab movement in doSomething.
    Actor* closestBehindCollLane(Actor* a); //finds the closest collision-avoidance worth actor behind a ZombieCab within 96 pixels. Returns nullptr if none found. Useful for ZombieCab movement in doSomething.
    GhostRacer* getOverlappingGhostRacer(Actor* a) const; // If actor a overlaps this world's GhostRacer, return a pointer to the GhostRacer; otherwise, return nullptr. Useful for dealing with collisions between GhostRacer and other Actors.
private:
    std::list<Actor*> aloa;
    GhostRacer* m_gr;
    int last_WB_y;
    int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH / 2;
    int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH / 2;
    int m_soulCount;
    int m_bonus;
    int lane(double x);
    bool overlap(Actor* a1, Actor* a2) const;
    Actor* closestBotCollLane(int l);
    Actor* closestTopCollLane(int l);
    
};

#endif // STUDENTWORLD_H_
