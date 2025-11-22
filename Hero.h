#ifndef HERO_H_INCLUDED
#define HERO_H_INCLUDED

#include "Object.h"
#include <string>
#include <map>

enum class Dir;

// fixed settings
enum class HeroState{
    LEFT,
    RIGHT,
    FRONT,
    BACK,
    HEROSTATE_MAX
};

/**
 * @brief The class of a hero.
 * @details Monster inherits Object and takes Rectangle as its hit box.
 */
class Hero : public Object
{
public:
    void init();
    void update();
    void draw() override;
private:
    HeroState state = HeroState::FRONT;
    double speed = 5;
    std::map<HeroState, std::string> gifPath;
};

#endif
