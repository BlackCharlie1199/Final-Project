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
    bool isInvincible() {return invincible;}
private:
    BossState state = BossState::IDLE;
    std::map<BossState,std::string>gifPath;
    double speedX, speedY;
    bool invincible = true;
    int hitCount = 0, bombs = 0;
    ALLEGRO_TIMER *bombCooldown;
    void move_and_throwBombs();
};

#endif