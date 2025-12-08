#ifndef FROG_H_INCLUDED
#define FROG_H_INCLUDED
#include "Object.h"
#include<map>
#include<string>
#include <allegro5/allegro.h>

enum class Dir {
    LEFT,
    RIGHT,
    UP,
    DOWN
};

enum class FrogState {
	RUN,
    JUMP,
    FALL,
    WALLJUMP,
    HIT,
    IDLE,
    DOUBLEJUMP,
    FROGSTATE_NUM
};

class Frog : public Object
{
public:
    void init();
    void update();
    void draw() override;

    Shape* getAttackShape() {return attackBox.get(); }
    bool slashEnd() {return currentFrame % 4 == 0; }
    void bounce(){}
private:
    FrogState state = FrogState::IDLE;
    std::map<FrogState,std::string>gifPath;
    std::string slashPath[4];
    int currentFrame;
    double speedX = 0, speedY = 0;
    bool attack, jump, doubleJump;
    
    Dir facing = Dir::RIGHT;
    Dir attackDir = Dir::RIGHT;
    std::shared_ptr<Shape> attackBox;
};

#endif