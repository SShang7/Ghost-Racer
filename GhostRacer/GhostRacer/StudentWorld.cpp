#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <sstream>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

int StudentWorld::init()
{
    m_soulCount = 0;
    m_bonus = 5000;
    m_gr = new GhostRacer(this);
    aloa.push_back(m_gr);
    int N = VIEW_HEIGHT / SPRITE_HEIGHT;
    for(int i = 0; i < N; ++i){
        int new_y = i * SPRITE_HEIGHT;
        Actor* left = new Border(this, true, LEFT_EDGE, new_y);
        aloa.push_back(left);
        Actor* right = new Border(this, true, RIGHT_EDGE, new_y);
        aloa.push_back(right);
    }
    int M = VIEW_HEIGHT / (4 * SPRITE_HEIGHT);
    for (int i = 0; i < M; i++) {
        Actor* left = new Border(this, false, LEFT_EDGE + ROAD_WIDTH / 3, i * (4 * SPRITE_HEIGHT));
        aloa.push_back(left);
        Actor* right = new Border(this, false, RIGHT_EDGE - ROAD_WIDTH / 3, i * (4 * SPRITE_HEIGHT));
        aloa.push_back(right);
    }
    last_WB_y = aloa.back()->getY();
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    for (list<Actor*>::iterator i = aloa.begin(); i != aloa.end(); ++i) {
        if ((*i)->getLiving()) {
            (*i)->doSomething();
            
            if (!m_gr->getLiving()) {
                decLives();
                return GWSTATUS_PLAYER_DIED;
            }
        }
        
    }
    //remove dead objects
    for (list<Actor*>::iterator i = aloa.begin(); i != aloa.end();) {
        if (!(*i)->getLiving()) {
            delete* i;
            i = aloa.erase(i);
        }
        else ++i;
    }
    //border spawn
    last_WB_y += -4 - m_gr->getSpeedV();
    int new_border_y = VIEW_HEIGHT - SPRITE_HEIGHT;
    int delta_y = new_border_y - last_WB_y;
    if (delta_y >= SPRITE_HEIGHT) {
        Actor* left = new Border(this, true, LEFT_EDGE, new_border_y);
        aloa.push_back(left);
        Actor* right = new Border(this, true, RIGHT_EDGE, new_border_y);
        aloa.push_back(right);
    }
    if (delta_y >= 4 * SPRITE_HEIGHT) {
        Actor* left = new Border(this, false, LEFT_EDGE + ROAD_WIDTH / 3, new_border_y);
        aloa.push_back(left);
        Actor* right = new Border(this, false, RIGHT_EDGE - ROAD_WIDTH / 3, new_border_y);
        aloa.push_back(right);
        last_WB_y = aloa.back()->getY();
    }
    int L = getLevel();
    //human ped spawn
    int ChanceHumanPed = max(200 - L * 10, 30);
    if (randInt(0, ChanceHumanPed - 1) == 0) {
        Actor* hp = new HumanPed(this, randInt(0, VIEW_WIDTH), VIEW_HEIGHT);
        aloa.push_back(hp);
    }
    //zombie ped spawn
    int ChanceZombiePed = max(100 - L * 10, 20);
    if (randInt(0, ChanceZombiePed - 1) == 0) {
        Actor* zp = new ZombiePed(this, randInt(0, VIEW_WIDTH), VIEW_HEIGHT);
        aloa.push_back(zp);
    }
    //oil slick spawn
    int ChanceOilSlick = max(150 - L * 10, 40);
    if (randInt(0, ChanceOilSlick - 1) == 0) {
        Actor* os = new OilSlick(this, randInt(LEFT_EDGE, RIGHT_EDGE), VIEW_HEIGHT);
        aloa.push_back(os);
    }
    //holy water spawn
    int ChanceOfHolyWater = 100 + 10 * L;
    if (randInt(0, ChanceOfHolyWater-1) == 0) {
        Actor* hw = new HolyWaterGoodie(this, randInt(LEFT_EDGE, RIGHT_EDGE), VIEW_HEIGHT);
        aloa.push_back(hw);
    }
    //lost soul spawn
    int ChanceOfLostSoul = 100;
    if (randInt(0, ChanceOfLostSoul-1) == 0) {
        Actor* ls = new Soul(this, randInt(LEFT_EDGE, RIGHT_EDGE), VIEW_HEIGHT);
        aloa.push_back(ls);
    }
    if (m_soulCount >= 2 * L + 5) {
        increaseScore(m_bonus);
        return GWSTATUS_FINISHED_LEVEL;
    }

    m_bonus--;
    ostringstream oss;
    oss << "Score: " << getScore();
    oss << " Lvl: " << L;
    oss << " Lives: " << getLives();
    oss << " Health: " << m_gr->getHP();
    oss << " Sprays: " << m_gr->getWater();
    oss << " Bonus: " << m_bonus;
    string s = oss.str();
    setGameStatText(s);
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for (list<Actor*>::iterator i = aloa.begin(); i != aloa.end();) {
        delete* i;
        i = aloa.erase(i);
    }
}

StudentWorld::~StudentWorld() {
    cleanUp();
}

GhostRacer* StudentWorld::getGR() const {
    return m_gr;
}

void StudentWorld::addActor(Actor* a) {
    aloa.push_back(a);
}
void StudentWorld::addSoul() {
    m_soulCount++;
}
bool StudentWorld::overlap(Actor* a1, Actor* a2) const {
    double delta_x = abs(a1->getX() - a2->getX());
    double delta_y = abs(a1->getY() - a2->getY());
    double radius_sum = a1->getRadius() + a2->getRadius();
    return delta_x < radius_sum * .25 && delta_y < radius_sum * .6;
}

bool StudentWorld::overlapWater(Actor* a) {
for (list<Actor*>::iterator i = aloa.begin(); i != prev(aloa.end()); ++i) {
    if (overlap(*i, a)) {
        bool shot = (*i)->sprayed();
        if (shot) return true;
    }
}
return false;
}

GhostRacer* StudentWorld::getOverlappingGhostRacer(Actor* a) const {
    if (overlap(a, m_gr)) {
        return m_gr;
    }
    return nullptr;
}