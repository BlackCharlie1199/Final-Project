#include <ctime>
#include <vector>
#include <cmath>
#include <cstdlib>
#include "data/GIFCenter.h"
#include "data/DATACenter.h"
#include "shapes/Rectangle.h"
#include "shapes/Point.h"
#include "Frog.h"
#include "Boss.h"
#include "bombs/Bomb.h"


namespace BossSetting {
	static constexpr char gif_root_path[40] = "./assets/gif/Boss/";
	static constexpr char dir_path_postfix[][15] = {
		"Run", "Hit", "Idle"
	};
    static constexpr int bomb_number = 3;
}

void Boss::init(){
    for(int i=0;i<static_cast<int>(BossState::BOSSSTATE_NUM);++i){
        char buffer[50];
        sprintf(buffer,"%smaskDude_%s.gif",
        BossSetting::gif_root_path,
        BossSetting::dir_path_postfix[static_cast<int>(i)]);
        gifPath[static_cast<BossState>(i)] = std::string{buffer};
    }
    GIFCenter *GIFC = GIFCenter::get_instance();
    ALGIF_ANIMATION *gif = GIFC->get(gifPath[state]);
    DataCenter *DC = DataCenter::get_instance();
    shape.reset(new Rectangle{DC->window_width/2,
                              DC->window_height/2,
                              DC->window_width/2 + gif->width,
                              DC->window_height/2 + gif->height});
    speedX = 0, speedY = 0;
    bombCooldown = al_create_timer(1 / DC->FPS);
    srand(time(NULL));
}

void Boss::update(){
    DataCenter *DC = DataCenter::get_instance();

    constexpr double gravity = 0.5;
    constexpr double fricition = 0.95;

    if (!invincible) {
        if (shape->center_y() < DC->window_height / 2) {
            speedY += gravity;
        } else if (speedY > 0) {
            speedY = 0;
            shape->update_center_y(DC->window_height / 2);
        }
        
        speedX *= fricition;

        shape->update_center_x(shape->center_x() + speedX);
        shape->update_center_y(shape->center_y() + speedY);

        if (state == BossState::HIT) return;
        state = BossState::IDLE;
    } else {
        al_start_timer(bombCooldown);
        move_and_throwBombs();
    }
}

void Boss::draw(){
    GIFCenter *GIFC = GIFCenter::get_instance();
    ALGIF_ANIMATION *gif = GIFC->get(gifPath[state]);

    algif_draw_gif(gif,
                   shape->center_x() - gif->width/2,
                   shape->center_y() - gif->height/2,
                   0);
    if (state == BossState::HIT) {
        if (gif->display_index == gif->frames_count - 1) {
            state = BossState::IDLE;
        }
    }
}

void Boss::hit(int sourceX) { 
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

void Boss::move_and_throwBombs() {
    if (al_get_timer_count(bombCooldown) <= 80) {
        return;
    }
    DataCenter *DC = DataCenter::get_instance();
    GIFCenter *GIFC = GIFCenter::get_instance();
    ALGIF_ANIMATION *gif = GIFC->get(gifPath[state]);

    Frog *frog = DC->frog;
    // every time boss throws 3 bombs, it will teleport to another place 
    if (bombs == 0) {
        double newY = std::max(rand() % (DC->window_height / 3), gif->height / 2);
        shape->update_center_x(frog->shape->center_x());
        shape->update_center_y(newY);
    }
    al_set_timer_count(bombCooldown, 0);
    // refer to the setting in bomb
    double upTime = 20;
    double g = 2 * 100 / (13.0 * 13.0);
    double dropTime = sqrt(2 * (DC->window_height / 2 - shape->center_y() - gif->height / 2 - 100) / g);
    double flyTime = upTime + dropTime;
    // throw bombs
    DC->bombs.emplace_back(new Bomb {Point{shape->center_x(), shape->center_y() - gif->height / 2}, 
                                     (frog->shape->center_x() - shape->center_x()) / flyTime});
    ++bombs;
    bombs %= BossSetting::bomb_number;
}