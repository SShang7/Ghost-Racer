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
    // doSomething on aloa if living
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
    //Zombie cab spawn
    int ChanceVehicle = max(100 - L * 10, 20);
    if (randInt(0, ChanceVehicle - 1) == 0) {
        int cur_lane = randInt(0, 2);
        double start_x;
        double start_y;
        int start_speed;
        bool skip = true;
        int lane_iter = 0;
        while (lane_iter < 3) {
            Actor* bot = closestBotCollLane(cur_lane);
            if (bot == nullptr || bot->getY() > VIEW_HEIGHT / 3) {
                start_y = SPRITE_HEIGHT / 2;
                start_speed = m_gr->getSpeedV() + randInt(2, 4);
                skip = false;
                break;
            }
            Actor* top = closestTopCollLane(cur_lane);
            if (top == nullptr || top->getY() < (VIEW_HEIGHT * 2 / 3)) {
                start_y = VIEW_HEIGHT - SPRITE_HEIGHT / 2;
                start_speed = m_gr->getSpeedV() - randInt(2, 4);
                skip = false;
                break;
            }
            
            if (cur_lane == 0) cur_lane = 1;
            else if (cur_lane == 1) cur_lane = 2;
            else cur_lane = 0;
            lane_iter++;
        }
        if (!skip) {
            switch (cur_lane) {
            case 0:
                start_x = ROAD_CENTER - ROAD_WIDTH/3;
                break;
            case 1:
                start_x = ROAD_CENTER;
                break;
            case 2:
                start_x = ROAD_CENTER + ROAD_WIDTH / 3;
                break;
            }
            Actor* zc = new ZombieCab(this, start_x, start_y, start_speed);
            aloa.push_back(zc);
        }
    }
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
    //soul requirement to pass level
    if (m_soulCount >= 2 * L + 5) {
        playSound(SOUND_FINISHED_LEVEL);
        increaseScore(m_bonus);
        return GWSTATUS_FINISHED_LEVEL;
    }
    //decrement bonus
    if (m_bonus > 0) {
        m_bonus--;
    }
    //score board
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
for (list<Actor*>::iterator i = aloa.begin(); i != aloa.end(); ++i) {
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
int StudentWorld::lane(double x) {
    int lane;
    if(x >= LEFT_EDGE && x < LEFT_EDGE + ROAD_WIDTH / 3) lane = 0;
    else if (x >= LEFT_EDGE + ROAD_WIDTH / 3 && x < RIGHT_EDGE - ROAD_WIDTH / 3) lane = 1;
    else lane = 2;
    return lane;
}
Actor* StudentWorld::closestFrontCollLane(Actor* a) {
    for (list<Actor*>::iterator i = aloa.begin(); i != aloa.end(); ++i) {
        if (lane(a->getX()) == lane((*i)->getX())) {
            if ((*i)->getColl()) {
                if ((*i)->getY() - a->getY() > 0) return *i;
            }
        }
    }
    return nullptr;
}
Actor* StudentWorld::closestBotCollLane(int l) {
    Actor* lowest = nullptr;
    for (list<Actor*>::iterator i = aloa.begin(); i != aloa.end(); ++i) {
        if (l == lane((*i)->getX())) {
            if ((*i)->getColl()) {
                if (lowest == nullptr) lowest = *i;
                else if ((*i)->getY() < lowest->getY()) lowest = *i;
            }
        }
    }
    return lowest;
}
Actor* StudentWorld::closestTopCollLane(int l) {
    Actor* highest = nullptr;
    for (list<Actor*>::iterator i = aloa.begin(); i != aloa.end(); ++i) {
        if (l == lane((*i)->getX())) {
            if ((*i)->getColl()) {
                if (highest == nullptr) highest = *i;
                else if ((*i)->getY() > highest->getY()) highest = *i;
            }
        }
    }
    return highest;
}