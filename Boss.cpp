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
    invincibleTimer = al_create_timer(1 / DC->FPS);
    al_set_timer_count(invincibleTimer, 50);
    al_start_timer(invincibleTimer);
}

void Boss::update(){
    //DataCenter *DC = DataCenter::get_instance();
    /*if(DC->key_state[ALLEGRO_KEY_W]){
        shape->update_center_y(shape->center_y() - speed);
        state = FrogState::JUMP;
    }
    else if(DC->key_state[ALLEGRO_KEY_A]){
        shape->update_center_x(shape->center_x() - speed);
        state = FrogState::RUN;
    }
    else if(DC->key_state[ALLEGRO_KEY_S]){
        shape->update_center_y(shape->center_y() + speed);
        state = HeroState::FRONT;
    }*/
   state = BossState::IDLE;
}

void Boss::draw(){
    GIFCenter *GIFC = GIFCenter::get_instance();
    ALGIF_ANIMATION *gif = GIFC->get(gifPath[state]);
    algif_draw_gif(gif,
                   shape->center_x() - gif->width/2,
                   shape->center_y() - gif->height/2,
                   0);
}