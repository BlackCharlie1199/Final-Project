#ifndef BOMB_H_INCLUDED
#define BOMB_H_INCLUDED

#include "../Object.h"
#include "allegro5/allegro.h"
#include "map"
#include "string"

enum class BombState {
	WALK,
    IDLE,
    EXPLODE,
    BOMBSTATE_NUM
};

class Bomb : public Object
{
public:
	Bomb(const Point &p);
    void update();
    void draw() override;
    bool isExploded();
private:
    BombState state = BombState::IDLE;
    std::map<BombState,std::string>gifPath;
    double speedX = 0, speedY = 0;
    ALLEGRO_TIMER *explodeTime;
    bool explode;
    int dmage = 10;
};

#endif