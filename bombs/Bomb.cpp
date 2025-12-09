#include <iostream>
#include "Bomb.h"
#include "../data/GIFCenter.h"
#include "../data/DATACenter.h"
#include "../shapes/Circle.h"
#include "../data/ImageCenter.h"

namespace BombSetting {
	static constexpr char gif_root_path[40] = "./assets/gif/Bomb";
	static constexpr char dir_path_postfix[][15] = {
		"Walk", "Idle", "Explode"
	};
    constexpr double ascend_height = 100;
    constexpr double ascend_time = 15;
    constexpr double descend_time = 13;
    constexpr double ascend_velocity = ((2 * ascend_height) / ascend_time) * -1;
    constexpr double ascend_gravity = ((2 * ascend_height) / (ascend_time * ascend_time));
    constexpr double descend_gravity = ((2 * ascend_height) / (descend_time * descend_time));
}

Bomb::Bomb(const Point &p, double speedX): speedX{speedX} {
    GIFCenter *GIFC = GIFCenter::get_instance();
    for(int i=0;i < static_cast<int>(BombState::BOMBSTATE_NUM);++i){
        char buffer[60];
        sprintf(buffer,"%s/bomb_%s.gif",
        BombSetting::gif_root_path,
        BombSetting::dir_path_postfix[static_cast<int>(i)]);
        gifPath[static_cast<BombState>(i)] = std::string{buffer};
    }
    ALGIF_ANIMATION *gif = GIFC->get(gifPath[state]);
    shape.reset(new Circle{p.x, p.y, static_cast<double>(gif->width)});
    explode = false;
    speedY = BombSetting::ascend_velocity;
}

void Bomb::update() {
    DataCenter *DC = DataCenter::get_instance();
    GIFCenter *GIFC = GIFCenter::get_instance();
    ALGIF_ANIMATION *gif = GIFC->get(gifPath[state]);
    double r = gif->width / 2;
    if (DC->mouse_state[2]) {
        state = BombState::EXPLODE;
    }

    if (shape->center_y() < DC->window_height / 2) {
        double g = BombSetting::ascend_gravity;
        if (speedY >= 0) {
            g = BombSetting::descend_gravity;
        }
        speedY += g;
    } else {
        shape->update_center_y(DC->window_height / 2);
        speedY *= - 0.5;

        if (std::abs(speedY) <= 2) {
            speedY = 0;
            speedX *= 0.9;
        }
    }

    if (shape->center_x() - r < 0) {
        shape->update_center_x(0 + r);
        speedX *= -1;
    } else if (shape->center_x() + r > DC->window_width) {
        shape->update_center_x(DC->window_width - r);
        speedX *= -1;
    }
    shape->update_center_x(shape->center_x() + speedX);
    shape->update_center_y(shape->center_y() + speedY);
}

void Bomb::draw(){
    GIFCenter *GIFC = GIFCenter::get_instance();
    ALGIF_ANIMATION *gif = GIFC->get(gifPath[state]);
    algif_draw_gif(gif,
                   shape->center_x() - gif->width/2,
                   shape->center_y() - gif->height/2,
                   0);
    if (state == BombState::EXPLODE && gif->display_index == gif->frames_count - 1) explode = true;
}
