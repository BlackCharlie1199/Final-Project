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
    void increaseHitCount() { 
        if (al_get_timer_count(invincibleTimer) >= 100) {
            ++hitCount; 
            if (hitCount % 5 == 0) {
                al_set_timer_count(invincibleTimer, 0);
                state = BossState::HIT;
            }
        }
    };
private:
    BossState state = BossState::IDLE;
    std::map<BossState,std::string>gifPath;
    double speed = 5;
    ALLEGRO_TIMER *invincibleTimer;
    int hitCount = 0;
};

#endif