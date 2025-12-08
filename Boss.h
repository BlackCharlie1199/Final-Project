#ifndef BOSS_H_INCLUDED
#define BOSS_H_INCLUDED
#include "Object.h"
#include<map>
#include<string>
#include <allegro5/allegro.h>

enum class BossState {
	RUN,
    HIT,
    IDLE,
    BOSSSTATE_NUM
};

class Boss : public Object
{
public:
    void init();
    void update();
    void draw() override;
    void hit(int sourceX) { 
        if (state == BossState::HIT) return;
        ++hitCount; 
        state = BossState::HIT;

        if (sourceX < shape->center_x()) {
            speedX = 10;
        } else {
            speedX = -10;
        }
        speedY = -10;
    };
private:
    BossState state = BossState::IDLE;
    std::map<BossState,std::string>gifPath;
    double speedX, speedY;
    int hitCount = 0;
};

#endif