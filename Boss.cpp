#include "Boss.h"
#include "data/GIFCenter.h"
#include "data/DATACenter.h"
#include "shapes/Rectangle.h"



namespace BossSetting {
	static constexpr char gif_root_path[40] = "./assets/gif/Boss/";
	static constexpr char dir_path_postfix[][15] = {
		"Run", "Hit", "Idle"
	};
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
}

void Boss::update(){
    DataCenter *DC = DataCenter::get_instance();

    constexpr double gravity = 0.5;
    constexpr double fricition = 0.95;

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