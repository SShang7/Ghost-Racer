#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
//Actor Constructor
Actor::Actor(StudentWorld* world, int imageID, double startX, double startY, double size, unsigned int depth, int startDirection, bool coll, int hp, int speedv, int speedh)
	: GraphObject(imageID, startX, startY, startDirection, size, depth), m_world(world), m_living(true), m_speedv(speedv), m_speedh(speedh), m_coll(coll), m_hp(hp)
{
}
int Actor::getHP() {
	return m_hp;
}
void Actor::loseHP(int n) {
	m_hp -= n;
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
	if (getHP() <= 0) {
		kill();
		getWorld()->playSound(SOUND_PLAYER_DIE);
		return;
	}
	if (getX() <= ROAD_CENTER - ROAD_WIDTH / 2) {
		if (getDirection() > 90) {
			loseHP(10);
			setDirection(82);
			getWorld()->playSound(SOUND_VEHICLE_CRASH);
		}
	}
	else if (getX() >= ROAD_CENTER + ROAD_WIDTH / 2) {
		if (getDirection() < 90) {
			loseHP(10);
			setDirection(98);
			getWorld()->playSound(SOUND_VEHICLE_CRASH);
		}
	}
	double pi = 3.1415926535;
	int ch;
	if (getWorld()->getKey(ch)) {
		int dir = getDirection();
		double speed = getSpeedV();
		
		switch (ch)
		{
		case KEY_PRESS_SPACE:
			if (m_water > 0) {
				StudentWorld* w = getWorld();
				Actor* s = new Spray(w, getX()+SPRITE_HEIGHT*sin((dir - 90) * pi/180), getY() + SPRITE_HEIGHT*cos((dir - 90) * pi/180), dir);
				w->addActor(s);
				w->playSound(SOUND_PLAYER_SPRAY);
				m_water--;
			}
			break;
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
		}
	}
	double max_shift_per_tick = 4.0;
	int direction = getDirection();
	double delta_x = cos(direction * pi / 180.0) * max_shift_per_tick;
	double cur_x = getX();
	double cur_y = getY();
	moveTo(cur_x + delta_x, cur_y);
	return;
}
int GhostRacer::getWater() const {
	return m_water;
}
void GhostRacer::addWater() {
	m_water+=10;
}
void GhostRacer::spin() {
	int new_dir;
	do {
		int exponent = randInt(0, 1);
		int delta_dir = pow(-1, exponent) * randInt(5, 25);
		new_dir = getDirection() + delta_dir;
	} while (new_dir < 60 || new_dir > 120);
	setDirection(new_dir);
}
//Spray Constructor
Spray::Spray(StudentWorld* world, double x, double y, int dir)
	: Actor(world, IID_HOLY_WATER_PROJECTILE, x, y, 1.0, 1, dir, false, 0, 0, 0)
{
	m_travelDist = 160;
}

void Spray::doSomething() {
	if (!getLiving()) return;
	//check activation
	moveForward(SPRITE_HEIGHT);
	m_travelDist -= SPRITE_HEIGHT;
	if (getX() < 0 || getY() < 0 || getX() > VIEW_WIDTH || getY() > VIEW_HEIGHT) {
		kill();
		return;
	}
	if (m_travelDist <= 0) {
		kill();
		return;
	}
}
//Stationary Constructor
Stationary::Stationary(StudentWorld* world, int imageID, double startX, double startY, double size, int dir)
	: Actor(world, imageID, startX, startY, size, 2, dir, false, 0, -4, 0){

}

//Border Constructor
Border::Border(StudentWorld* world, bool yellow, int x, int y)
	: Stationary(world, isYellowImage(yellow), x, y, 2.0, 0) {

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

//OilSlick Constructor
OilSlick::OilSlick(StudentWorld* world, double x, double y) : Stationary(world, IID_OIL_SLICK, x, y, randInt(2, 5), 0) {
}

//OilSlick doSomething
void OilSlick::doSomething() {
	double vert_speed = -4.0 - getWorld()->getGR()->getSpeedV();
	double horiz_speed = 0;
	double new_y = getY() + vert_speed;
	double new_x = getX() + horiz_speed;
	moveTo(new_x, new_y);
	if (getX() < 0 || getY() < 0 || getX() > VIEW_WIDTH || getY() > VIEW_HEIGHT) {
		kill();
		return;
	}
	GhostRacer* gr = getWorld()->getOverlappingGhostRacer(this);
	if (gr != nullptr) {
		getWorld()->playSound(SOUND_OIL_SLICK);
		gr->spin();
	}

}
//HolyWaterGoodie Constructor
HolyWaterGoodie::HolyWaterGoodie(StudentWorld* world, double x, double y) : Stationary(world, IID_HOLY_WATER_GOODIE, x, y, 2.0, 90){
	
}
//HolyWaterGoodie doSomething
void HolyWaterGoodie::doSomething() {
	double vert_speed = -4.0 - getWorld()->getGR()->getSpeedV();
	double horiz_speed = 0;
	double new_y = getY() + vert_speed;
	double new_x = getX() + horiz_speed;
	moveTo(new_x, new_y);
	if (getX() < 0 || getY() < 0 || getX() > VIEW_WIDTH || getY() > VIEW_HEIGHT) {
		kill();
		return;
	}
	GhostRacer* gr = getWorld()->getOverlappingGhostRacer(this);
	if (gr != nullptr) {
		gr->addWater();
		kill();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(50);
	}
	
}
//Soul Constructor
Soul::Soul(StudentWorld* world, double x, double y) : Stationary(world, IID_SOUL_GOODIE, x, y, 4.0, 0) {

}
void Soul::doSomething() {
	double vert_speed = -4.0 - getWorld()->getGR()->getSpeedV();
	double horiz_speed = 0;
	double new_y = getY() + vert_speed;
	double new_x = getX() + horiz_speed;
	moveTo(new_x, new_y);
	if (getX() < 0 || getY() < 0 || getX() > VIEW_WIDTH || getY() > VIEW_HEIGHT) {
		kill();
		return;
	}
	GhostRacer* gr = getWorld()->getOverlappingGhostRacer(this);
	if (gr != nullptr) {
		getWorld()->addSoul();
		kill();
		getWorld()->playSound(SOUND_GOT_SOUL);
		getWorld()->increaseScore(100);
	}
	int dir = getDirection();
	setDirection(dir + 10);
}
//Pedestrian Constructor
Pedestrian::Pedestrian(StudentWorld* world, int imageID, double startX, double startY, double size, int speedv) :
	Actor(world, imageID, startX, startY, size, 0, 90, true, 2, -4, 0)
{
	m_plan = 0;
}

//ZombiePed Constructor
