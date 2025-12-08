#include <iostream>
#include "Frog.h"
#include "data/GIFCenter.h"
#include "data/DATACenter.h"
#include "shapes/Rectangle.h"
#include "data/ImageCenter.h"
#include <allegro5/allegro_primitives.h>
#include "bombs/Bomb.h"



namespace FrogSetting {
	static constexpr char gif_root_path[40] = "./assets/gif/Frog";
	static constexpr char dir_path_postfix[][15] = {
		"Run", "Jump", "Fall", "WallJump", "Hit", "Idle", "DoubleJump"
	};
    const int slashFrames = 4;
    const int color = 4;
    const int attackCoolDown = 20;
    constexpr double jump_height = 200;
    constexpr double jump_time_to_peak = 22;
    constexpr double jump_time_to_descent = 20;
    constexpr double jump_velocity = ((2 * jump_height) / jump_time_to_peak) * -1;
    constexpr double jump_gravity = ((2 * jump_height) / (jump_time_to_peak * jump_time_to_peak));
    constexpr double fall_gravity = ((2 * jump_height) / (jump_time_to_descent * jump_time_to_descent));

    constexpr int attack_w = 60;
    constexpr int attack_h = 60;
    constexpr int attack_offset = 50;
}

void Frog::init(){
    for(int i=0;i<static_cast<int>(FrogState::FROGSTATE_NUM);++i){
        char buffer[50];
        sprintf(buffer,"%s/ninjaFrog_%s.gif",
        FrogSetting::gif_root_path,
        FrogSetting::dir_path_postfix[static_cast<int>(i)]);
        gifPath[static_cast<FrogState>(i)] = std::string{buffer};
    }
    for (int i = 1; i <= FrogSetting::slashFrames; ++i) {
        char buffer[70];
        sprintf(buffer, "%s/Slash_color%d_frame%d.png",
                FrogSetting::gif_root_path,
                FrogSetting::color,
                i);
        slashPath[i - 1] = std::string{buffer};
    }
    GIFCenter *GIFC = GIFCenter::get_instance();
    ALGIF_ANIMATION *gif = GIFC->get(gifPath[state]);
    DataCenter *DC = DataCenter::get_instance();
    shape.reset(new Rectangle{DC->window_width/2,
                              DC->window_height/2,
                              DC->window_width/2 + gif->width,
                              DC->window_height/2 + gif->height});
    attackBox.reset(new Rectangle{0, 0, 0, 0});
    attack = false;
    currentFrame = 0;
    doubleJump = false;
}

void Frog::update(){
    DataCenter *DC = DataCenter::get_instance();
    GIFCenter *GIFC = GIFCenter::get_instance();
    ALGIF_ANIMATION *gif = GIFC->get(gifPath[state]);
    auto getGravity = [](double speedY) {
        return speedY > 0 ? FrogSetting::fall_gravity : FrogSetting::jump_gravity;
    };


    if (shape->center_y() >= DC->window_height / 2) {
        speedY = 0;
        state = FrogState::IDLE;
        doubleJump = false;
    }

    bool inAir = state == FrogState::JUMP || state== FrogState::FALL || state == FrogState::DOUBLEJUMP;
    if (DC->key_state[ALLEGRO_KEY_SPACE] && !DC->prev_key_state[ALLEGRO_KEY_SPACE] && inAir && !doubleJump) {
        if (state != FrogState::WALLJUMP) {
            speedY = FrogSetting::jump_velocity;
            state = FrogState::DOUBLEJUMP;
            inAir = true;
            doubleJump = true;
        }
    }

    bool horMove = false;

    bool reachLeft = shape->center_x() - gif->width / 2 + 20 <= 0;
    bool reachRight = shape->center_x() + gif->width / 2 - 20 >= DC->window_width;

    if(DC->key_state[ALLEGRO_KEY_A] && !reachLeft){
        speedX = -10;
        if (state != FrogState::WALLJUMP) facing = Dir::LEFT;
        if (!inAir) {
            state = FrogState::RUN;
        }
        horMove = true;
    } else if(DC->key_state[ALLEGRO_KEY_D] && !reachRight){
        speedX = 10;
        if (state != FrogState::WALLJUMP) facing = Dir::RIGHT;
        if (!inAir) {
            state = FrogState::RUN;
        }
        horMove = true;
    } else {
        speedX = 0;
    }

    bool onLeftWall = (reachLeft && shape->center_y() <= DC->window_height / 2);
    bool onRightWall = (reachRight && shape->center_y() <= DC->window_height / 2);

    // handle wall jump
    if ((onLeftWall || onRightWall) && speedY > 0) {
        state = FrogState::WALLJUMP;
        doubleJump = false;
        speedX = 0;
        speedY = 2;
    }

    if (!inAir || state == FrogState::WALLJUMP) {
        if(DC->key_state[ALLEGRO_KEY_SPACE]){
            if (state == FrogState::WALLJUMP) {
                speedY = FrogSetting::jump_velocity;
                state = FrogState::JUMP;
                if (onLeftWall) {
                    speedX = 15;
                    facing = Dir::RIGHT;
                } else if (onRightWall) {
                    speedX = -15;
                    facing = Dir::LEFT;
                }
            } else{
                speedY = FrogSetting::jump_velocity;
                state = FrogState::JUMP;
            }
        } else if (!horMove && state != FrogState::WALLJUMP) {
            state = FrogState::IDLE;
        }
    }

    // attack part
    if (DC->mouse_state[1] && currentFrame == 0 && !DC->prev_mouse_state[1]) {
        attack = true;
        if (DC->key_state[ALLEGRO_KEY_W]) {
            attackDir = Dir::UP;
        } else if (DC->key_state[ALLEGRO_KEY_S]) {
            attackDir = Dir::DOWN;
        } else {
            attackDir = facing;
        }
    } else {
        if (currentFrame != 0) attack = true;
        else attack = false;
    }

    if (attack) {
        double attackCenterX = shape->center_x();
        double attackCenterY = shape->center_y() + 10;

        switch (attackDir) {
            case Dir::RIGHT: attackCenterX += FrogSetting::attack_offset + 15; break;
            case Dir::LEFT : attackCenterX -= FrogSetting::attack_offset + 15; break;
            case Dir::UP:    attackCenterY -= FrogSetting::attack_offset + 30; break;
            case Dir::DOWN : attackCenterY += FrogSetting::attack_offset + 15; break;
        }

        attackBox->update_center_x(attackCenterX);
        attackBox->update_center_y(attackCenterY);
    } else {
        attackBox->update_center_x(-1000);
        attackBox->update_center_y(-1000);
    }

    if (inAir) {
        if (state != FrogState::WALLJUMP)  speedY += getGravity(speedY); 
        if (!(onLeftWall || onRightWall) && state == FrogState::WALLJUMP)  state = FrogState::FALL; 
        if (speedY > 0 && state != FrogState::WALLJUMP) state = FrogState::FALL;
    }

    shape->update_center_y(shape->center_y() + speedY);
    shape->update_center_x(shape->center_x() + speedX);
}

void Frog::draw(){
    GIFCenter *GIFC = GIFCenter::get_instance();
    ImageCenter *IMGC = ImageCenter::get_instance();
    ALGIF_ANIMATION *gif = GIFC->get(gifPath[state]);

    int gifFlags = (facing == Dir::LEFT) ? ALLEGRO_FLIP_HORIZONTAL : 0;
    algif_draw_gif(gif,
                   shape->center_x() - gif->width/2,
                   shape->center_y() - gif->height/2,
                   gifFlags);
    if (attack) {
        const int frameNum = 6;
        ALLEGRO_BITMAP *bmp = IMGC->get(slashPath[currentFrame / frameNum]);

        double angle = 0;
        int flags = 0;

        switch(attackDir) {
            case Dir::RIGHT: 
                angle = 0;
                break;
            case Dir::LEFT: 
                angle = 0;
                flags = ALLEGRO_FLIP_HORIZONTAL;
                break;
            case Dir::UP:
                angle = -ALLEGRO_PI / 2; // -90
                flags = 0;
                break;
            case Dir::DOWN:
                angle = ALLEGRO_PI / 2; // 90
                flags = 0;
                break;
        }

        float w = al_get_bitmap_width(bmp);
        float h = al_get_bitmap_height(bmp);
        float drawX = shape->center_x();
        float drawY = shape->center_y();
        
        switch (attackDir) {
            case Dir::RIGHT: drawX += 40; drawY += 5; break; 
            case Dir::LEFT:  drawX -= 40; drawY += 5; break; 
            case Dir::UP:    drawX -= 10; drawY -= 65; break;
            case Dir::DOWN:  drawY += 65; break;
        }
        al_draw_rotated_bitmap(bmp, w / 2, h / 2, drawX, drawY, angle, flags);
        //al_draw_rectangle(attackBox->center_x() - 30, attackBox->center_y() - 30, 
                        //attackBox->center_x() + 30, attackBox->center_y() + 30, 
                        //al_map_rgb(255, 0, 0), 2);
        ++currentFrame;
        currentFrame %= FrogSetting::slashFrames * frameNum;
    }
}

void Frog::bounce(){
    
}