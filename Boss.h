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
    void hit(int sourceX);
private:
    BossState state = BossState::IDLE;
    std::map<BossState,std::string>gifPath;
    double speedX, speedY;
    int hitCount = 0;
};

#endif