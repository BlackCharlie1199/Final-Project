#include "Hero.h"
#include "data/DataCenter.h"
#include "data/GIFCenter.h"
#include "algif5/algif.h"
#include "shapes/Rectangle.h"

namespace HeroSetting
{
    static constexpr char gif_root_path[50] = "./assets/gif/Hero/";
    static constexpr char gif_postfix[][10] = {
        "left",
        "right",
        "front",
        "back",
    };
} // namespace HeroSetting

void Hero::init()
{
    for (size_t type = 0; type < static_cast<size_t>(HeroState::HEROSTATE_MAX); ++type) {
        char buffer[50];
        sprintf(buffer, "%s/dragonite_%s.gif",
                HeroSetting::gif_root_path,
                HeroSetting::gif_postfix[static_cast<int>(type)]);
        gifPath[static_cast<HeroState>(type)] = std::string{buffer};
    }
    GIFCenter *GIFC = GIFCenter::get_instance();
    ALGIF_ANIMATION *gif = GIFC->get(gifPath[state]);
    DataCenter *DC = DataCenter::get_instance();
    shape.reset(new Rectangle{DC->window_width / 2,
                              DC->window_height / 2,
                              DC->window_width / 2 + gif->width,
                              DC->window_height / 2 + gif->height});
}

void Hero::draw() {
    GIFCenter *GIFC = GIFCenter::get_instance();
    ALGIF_ANIMATION *gif = GIFC->get(gifPath[state]);
    algif_draw_gif(gif,
                   shape->center_x() - gif->width / 2,
                   shape->center_y() - gif->height / 2,
                   0);
}