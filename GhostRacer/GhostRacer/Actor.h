#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;
//Actor Class
class Actor : public GraphObject {
private:
	bool m_living;
	double m_speedv;
	double m_speedh;
	bool m_coll;
	int m_hp;
	StudentWorld* m_world;

public:
	Actor(int imageID, double startX, double startY, double size, unsigned int depth, int startDirection, bool coll, int hp, int speedv, int speedh);
	virtual void doSomething() = 0;
};


//Car Class
class Car : public Actor {
	public:
		Car(int imageID, double startX, double startY, int hp, int speedv);
		virtual void doSomething() = 0;
};


//GhostRacer Class
class GhostRacer : public Car {
public:
	GhostRacer();
	void doSomething() = 0;
private:
	int m_water;
};



class Stationary : Actor {
public:
	Stationary(int imageID, double startX, double startY, double size, int hp, int speedv, int speedh);
	virtual void doSomething() = 0;

};

class Border : Stationary {
public:
	Border(bool yellow, int x, int y);
	void doSomething() = 0;
private:
	int isYellowImage(bool yellow);
};
/*
//ZombieCab Class
class ZombieCab : public Car {
public:
	ZombieCab();
	void doSomething() = 0;
};
*/
//Pedestrian Class
class Pedestrian : public Actor {
public:
	Pedestrian(int imageID, double startX, double startY, double size, int speedv);
	virtual void doSomething() = 0;
private:
	int m_plan;
};

/*
//Human Class
class Human : public Pedestrian {
public:
	Human();
	void doSomething() = 0;
};
*/
#endif // ACTOR_H_