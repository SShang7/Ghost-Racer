#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();

private:
    int m_level;
    int m_score;
    int m_bonus;
    int m_souls2save;
    int m_lives;
    int m_health;
    int m_sprays;
};

#endif // STUDENTWORLD_H_
