#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
//Actor Constructor
Actor::Actor(int imageID, double startX, double startY, double size, unsigned int depth, int startDirection, bool coll, int hp, int speedv, int speedh)
	: GraphObject(imageID, startX, startY, startDirection, size, depth), m_living(true), m_speedv(speedv), m_speedh(speedh), m_coll(coll), m_hp(hp)
{
}

//Car Constructor
Car::Car(int imageID, double startX, double startY, int hp, int speedv) 
	: Actor(imageID, startX, startY, 4.0, 0, 90, true, hp, speedv, 0) {
}

//GhostRacer Constructor
GhostRacer::GhostRacer() : Car(IID_GHOST_RACER, 128, 32, 100, 0) {
	m_water = 10;
}

//Stationary Constructor
Stationary::Stationary(int imageID, double startX, double startY, double size, int hp, int speedv, int speedh) 
	: Actor(imageID, startX, startY, size, 2, 0, false, hp, -4, 0){

}

//Border Constructor
Border::Border(bool yellow, int x, int y)
	: Stationary(isYellowImage(yellow), x, y, 2.0, 0, -4, 0) {

}
int Border::isYellowImage(bool yellow){
	if (yellow) return IID_YELLOW_BORDER_LINE;
	else return IID_WHITE_BORDER_LINE;
}
//Pedestrian Constructor
Pedestrian::Pedestrian(int imageID, double startX, double startY, double size, int speedv) :
	Actor(imageID, startX, startY, size, 0, 90, true, 2, -4, 0)
{
	m_plan = 0;
}
