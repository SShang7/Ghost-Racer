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
	void setSpeedV(double s);
	void setSpeedH(double s);
	double getSpeedH() const;
	StudentWorld* getWorld() const;
public:
	Actor(StudentWorld* world, int imageID, double startX, double startY, double size, unsigned int depth, int startDirection, bool coll, int hp, int speedv, int speedh);
	virtual void doSomething() = 0; //pure virtual function because actors deriving from this class will have their own doSomethings
	virtual bool sprayed(); //defaults to return false for objects that don't interact with the Holy Water projectile. Virtual because certain actors have their own sprayed interactions.
	int getHP() const; //gets the HP of the actor
	void loseHP(int n); //subtracts n from HP for when an actor takes damage
	void setHP(int n); //sets the HP to the value of n
	bool getLiving() const; //lets you know if the Actor is still alive
	void kill();//Turns the Actor's m_living = false;
	double getSpeedV() const;// gets the vertical speed of the actor
	bool getColl() const;// gets if the actor is a collision-avoidance worthy actor
};


//Car Class
class Car : public Actor {
	public:
		Car(StudentWorld* world, int imageID, double startX, double startY, int hp, int speedv); //Constructor for Car. All cars have the same size = 4.0, direction = 90, and are collision-avoidance worthy actors.
		virtual void doSomething() = 0; //Pure virtual because each Car has it's own doSomething.
};


//GhostRacer Class
class GhostRacer : public Car {
public:
	GhostRacer(StudentWorld* world); //constructor for GhostRacer. GhostRacer already has imageID, an x and y, hp , and speedv.
	virtual void doSomething(); //GhostRacer's doSomething. Checks for keypresses and moves the GhostRacer.
	void addWater(); //addWater increments the number of sprays by 10.
	void spin(); //movement interaction for GhostRacer when it touches an OilSlick
	int getWater() const; //gets the number of sprays that GhostRacer has left.
private:
	int m_water;
};

//Spray Class
class Spray : public Actor
{
public:
	Spray(StudentWorld* world, double x, double y, int dir); //constructor for Spray. Spray gets the world, x, y, and dir from GhostRacer doSomething.
	virtual void doSomething(); //updates the spray movement and checks if the spray hits any other actors.
private:
	int m_travelDist;
};

//Stationary Class
class Stationary : public Actor {
public:
	Stationary(StudentWorld* world, int imageID, double startX, double startY, double size, int dir); //Stationary Constructor for elements that don't move.

};

//Border Class
class Border : public Stationary {
public:
	Border(StudentWorld* world, bool yellow, int x, int y); //Border constructor that takes in the world, if the border should be yellow or white, and x and y positions.
	virtual void doSomething(); //Border's doSomething which is to just stay still.
private:
	int isYellowImage(bool yellow);
};
//class OilSlick
class OilSlick : public Stationary {
public: 
	OilSlick(StudentWorld* world, double x, double y); //OilSlick constructor that takes in world, x, and y. Makes size randInt(2,5).
	virtual void doSomething(); //OilSlick's doSomething stays still and checks for collisions with GhostRacer. Calls GhostRacer's spin if collide.
};
//HealingGoodie Class
class HealingGoodie : public Stationary
{
public:
	HealingGoodie(StudentWorld* world, double x, double y); //HealingGoodie constructor that takes in world, x, and y from dead ZombiePed's world, x, and y.
	virtual void doSomething(); //HealingGoodie doSomething stays still and checks for collisions with GhostRacer. Heals GhostRacer if collide. Makes sure no overheal > 100hp and increase score.
	virtual bool sprayed(); //calls kill() and returns true if sprayed.
};
//HolyWaterGoodie Class
class HolyWaterGoodie : public Stationary
{
public:
	HolyWaterGoodie(StudentWorld* world, double x, double y); //HolyWaterGoodie constructor that takes in world, x, and y from StudentWorld's move().
	virtual void doSomething();//HolyWaterGoodie doSomething stays still and checks for collisions with GhostRacer. Adds water to GhostRacer if collide and increase score.
	virtual bool sprayed();//calls kill() and returns true if sprayed.
};

//Soul Class
class Soul : public Stationary
{
public:
	Soul(StudentWorld* world, double x, double y);//Soul constructor that takes in world, x, and y from StudentWorld's move().
	virtual void doSomething();//Soul doSomething stays still and rotates direction. Checks for collisions with GhostRacer. Adds a soul to StudentWorld if collide and increase score.
};


//ZombieCab Class
class ZombieCab : public Car {
public:
	ZombieCab(StudentWorld* world, double x, double y, int speedv); //ZombieCab constructor that takes in world, x, y, and speedv from StudentWorld move().
	virtual void doSomething(); //doSomething updates its position and has ZombieCab's movement algorithms.
	virtual bool sprayed();//loses one hp if hit by water. if killed by water has chance to drop oil spill.
private:
	bool m_damaged;
	int m_plan;
};

//Pedestrian Class
class Pedestrian : public Actor {
public:
	Pedestrian(StudentWorld* world, int imageID, double startX, double startY, double size); //Pedestrian constructor that initializes common values for Pedestrians and starts move_plan at 0
	virtual void doSomething() = 0; // doSomething is pure virtual because each Pedestrian has it's own doSomething
protected:
	void moveDec();
	void newPlan();
	int getMove() const;
	void setMove(int n);
private:
	int m_plan;
};

//HumanPed Class
class HumanPed : public Pedestrian {
public:
	HumanPed(StudentWorld* world, double x, double y); //constructor for HumanPed at a certain x and y from StudentWorld
	virtual void doSomething(); //doSomething for HumanPed that updates its movement and detects if it is hit by a GhostRacer
	virtual bool sprayed(); //sprayed makes it reverse direction and returns true
};

//ZombiePed Class
class ZombiePed : public Pedestrian {
public:
	ZombiePed(StudentWorld* world, double x, double y); //constructor for ZombiePed at a certain x and y from StudentWorld
	virtual void doSomething();//doSomething for ZombiePed that updates its movement and detects if it is hit by a GhostRacer
	virtual bool sprayed(); //sprayed makes it take 1hp damage and if it kills there is a chance of heal goodie drop. Returns true.
private:
	int m_grunt;
};

#endif // ACTOR_H_