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
protected:
	void relativeSpeed();
	bool removeOutofBounds();
public:
	Actor(StudentWorld* world, int imageID, double startX, double startY, double size, unsigned int depth, int startDirection, bool coll, int hp, int speedv, int speedh);
	virtual void doSomething() = 0;
	virtual bool sprayed();
	int getHP() const;
	void loseHP(int n);
	void setHP(int n);
	bool getLiving() const;
	void kill();
	double getSpeedV() const;
	void setSpeedV(double s);
	double getSpeedH() const;
	void setSpeedH(double s);
	bool getColl() const;
	StudentWorld* getWorld() const;
};


//Car Class
class Car : public Actor {
	public:
		Car(StudentWorld* world, int imageID, double startX, double startY, int hp, int speedv);
		virtual void doSomething() = 0;
};


//GhostRacer Class
class GhostRacer : public Car {
public:
	GhostRacer(StudentWorld* world);
	virtual void doSomething();
	void addWater();
	void spin();
	int getWater() const;
private:
	int m_water;
};

//Spray Class
class Spray : public Actor
{
public:
	Spray(StudentWorld* world, double x, double y, int dir);
	virtual void doSomething();
private:
	int m_travelDist;
};

//Stationary Class
class Stationary : public Actor {
public:
	Stationary(StudentWorld* world, int imageID, double startX, double startY, double size, int dir);

};

//Border Class
class Border : public Stationary {
public:
	Border(StudentWorld* world, bool yellow, int x, int y);
	virtual void doSomething();
private:
	int isYellowImage(bool yellow);
};
//class OilSlick
class OilSlick : public Stationary {
public: 
	OilSlick(StudentWorld* world, double x, double y);
	virtual void doSomething();
};
//HealingGoodie Class
class HealingGoodie : public Stationary
{
public:
	HealingGoodie(StudentWorld* world, double x, double y);
	virtual void doSomething();
	virtual bool sprayed();
};
//HolyWaterGoodie Class
class HolyWaterGoodie : public Stationary
{
public:
	HolyWaterGoodie(StudentWorld* world, double x, double y);
	virtual void doSomething();
	virtual bool sprayed();
};

//Soul Class
class Soul : public Stationary
{
public:
	Soul(StudentWorld* world, double x, double y);
	virtual void doSomething();
};


//ZombieCab Class
class ZombieCab : public Car {
public:
	ZombieCab(StudentWorld* world, double x, double y, int speedv);
	virtual void doSomething();
	virtual bool sprayed();
private:
	bool m_damaged;
	int m_plan;
};

//Pedestrian Class
class Pedestrian : public Actor {
public:
	Pedestrian(StudentWorld* world, int imageID, double startX, double startY, double size);
	virtual void doSomething() = 0;
	virtual bool sprayed() = 0;
	int getMove() const;
	void setMove(int n);
protected:
	void moveDec();
	void newPlan();
private:
	int m_plan;
};

//HumanPed Class
class HumanPed : public Pedestrian {
public:
	HumanPed(StudentWorld* world, double x, double y);
	virtual void doSomething();
	virtual bool sprayed();
};

//ZombiePed Class
class ZombiePed : public Pedestrian {
public:
	ZombiePed(StudentWorld* world, double x, double y);
	virtual void doSomething();
	virtual bool sprayed();
private:
	int m_grunt;
};

#endif // ACTOR_H_