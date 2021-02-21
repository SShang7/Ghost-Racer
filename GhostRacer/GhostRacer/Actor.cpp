#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
//Actor Constructor
Actor::Actor(StudentWorld* world, int imageID, double startX, double startY, double size, unsigned int depth, int startDirection, bool coll, int hp, int speedv, int speedh)
	: GraphObject(imageID, startX, startY, startDirection, size, depth), m_world(world), m_living(true), m_speedv(speedv), m_speedh(speedh), m_coll(coll), m_hp(hp)
{
}

bool Actor::getLiving() {
	return m_living;
}
void Actor::kill() {
	m_living = false;
}
double Actor::getSpeedV() {
	return m_speedv;
}
void Actor::setSpeedV(double s) {
	m_speedv = s;
}
double Actor::getSpeedH() {
	return m_speedh;
}
StudentWorld* Actor::getWorld() {
	return m_world;
}
//Car Constructor
Car::Car(StudentWorld* world, int imageID, double startX, double startY, int hp, int speedv) 
	: Actor(world, imageID, startX, startY, 4.0, 0, 90, true, hp, speedv, 0) {
}

//GhostRacer Constructor
GhostRacer::GhostRacer(StudentWorld* world) : Car(world, IID_GHOST_RACER, 128, 32, 100, 0) {
	m_water = 10;
}


//GhostRacer doSomething()
void GhostRacer::doSomething() {
	if (!getLiving()) return;
	int ch;
	if (getWorld()->getKey(ch)) {
		int dir = getDirection();
		double speed = getSpeedV();
		switch (ch)
		{
		case KEY_PRESS_LEFT:
			if (dir < 114)
				setDirection(dir + 8);
			break;
		case KEY_PRESS_RIGHT:
			if (dir > 66)
				setDirection(dir - 8);
			break;
		case KEY_PRESS_UP:
			if (speed < 5)
				setSpeedV(speed + 1);
			break;
		case KEY_PRESS_DOWN:
			if (speed > -1)
				setSpeedV(speed - 1);
			break;
		case KEY_PRESS_SPACE:
			if (m_water > 0) {
				m_water--;
			}
			break;
		}
		
	}
	double max_shift_per_tick = 4.0;
	int direction = getDirection();
	double delta_x = cos(direction * 3.14159264535 / 180) * max_shift_per_tick;
	int cur_x = getX();
	int cur_y = getY();
	moveTo(cur_x + delta_x, cur_y);
	return;
}

//Stationary Constructor
Stationary::Stationary(StudentWorld* world, int imageID, double startX, double startY, double size, int hp, int speedv, int speedh)
	: Actor(world, imageID, startX, startY, size, 2, 0, false, hp, -4, 0){

}

//Border Constructor
Border::Border(StudentWorld* world, bool yellow, int x, int y)
	: Stationary(world, isYellowImage(yellow), x, y, 2.0, 0, -4, 0) {

}
int Border::isYellowImage(bool yellow){
	if (yellow) return IID_YELLOW_BORDER_LINE;
	else return IID_WHITE_BORDER_LINE;
}
void Border::doSomething() {
	double vert_speed = -4.0 - getWorld()->getGR()->getSpeedV();
	double horiz_speed = getSpeedH();
	int new_y = getY() + vert_speed;
	int new_x = getX() + horiz_speed;
	moveTo(new_x, new_y);
	if (getX() < 0 || getY() < 0 || getX() > VIEW_WIDTH || getY() > VIEW_HEIGHT) {
		kill();
		return;
	}
}
//Pedestrian Constructor
Pedestrian::Pedestrian(StudentWorld* world, int imageID, double startX, double startY, double size, int speedv) :
	Actor(world, imageID, startX, startY, size, 0, 90, true, 2, -4, 0)
{
	m_plan = 0;
}
