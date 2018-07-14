#ifdef ANDROID
#include "../touch_input.hpp"
#include "../hsp.hpp"
#endif

namespace elona
{
namespace snail
{

#include <android/log.h>

#define  LOG_TAG    "ElonaFoobar"

#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)

touch_input& touch_input::instance()
{
    static touch_input the_instance;
    return the_instance;
}

void touch_input::initialize(const fs::path& asset_path)
{
    if (false)
    {
        joystick_image_ = std::make_unique<basic_image>(asset_path / "joystick.png");
    }
    quick_action_image_ = std::make_unique<basic_image>(asset_path / "quick_action.png");

    initialize_quick_actions();

    initialized_ = true;
}

void touch_input::initialize_quick_actions()
{
    int width = application::instance().physical_width();
    int height = application::instance().physical_height();
    const constexpr int size = 300;
    int space_between = size * 0.75;

    quick_actions_.clear();

    std::vector<std::pair<std::string, snail::key>> keys =
        {{"7", snail::key::keypad_7},
         {"8", snail::key::keypad_8},
         {"9", snail::key::keypad_9},
         {"4", snail::key::keypad_4},
         {"5", snail::key::keypad_5},
         {"6", snail::key::keypad_6},
         {"1", snail::key::keypad_1},
         {"2", snail::key::keypad_2},
         {"3", snail::key::keypad_3}};

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            auto pair = keys.at(j + ((2-i) * 3));
            std::string name = pair.first;
            snail::key key = pair.second;

            int x = space_between + (j * space_between);
            int y = height - space_between - (i * space_between);

            quick_actions_.emplace_back(name, key, x, y);
        }
    }

    quick_actions_.emplace_back("OK", snail::key::enter,   width - space_between * 1, height - space_between * 1);
    quick_actions_.emplace_back("Back", snail::key::shift, width - space_between * 2, height - space_between * 1);
    quick_actions_.emplace_back("z", snail::key::key_z,    width - space_between * 1, height - space_between * 2);
    quick_actions_.emplace_back("x", snail::key::key_x,    width - space_between * 2, height - space_between * 2);
    quick_actions_.emplace_back("Esc", snail::key::escape, width - space_between * 1, height - space_between * 4);
}

void touch_input::draw_quick_actions()
{
    if (!initialized_) {
        return;
    }
    for (auto it = quick_actions_.begin(); it < quick_actions_.end(); it++)
    {
        draw_quick_action(*it);
    }
}

static bool is_touched(int x, int y, const quick_action& action)
{
    const constexpr int size = 300/2;
    int deadzone = (size * 0.75);

    return x >  action.center_x - deadzone
        && y >  action.center_y - deadzone
        && x <= action.center_x + deadzone
        && y <= action.center_y + deadzone;
}

void touch_input::draw_quick_action(const quick_action& action)
{
    const constexpr int size = 300/2;
    int deadzone = (size * 0.75);
    int x = action.center_x - deadzone;
    int y = action.center_y - deadzone;
    auto& renderer = application::instance().get_renderer();

    renderer.render_image(*quick_action_image_.get(), x, y, size * 1.5, size * 1.5);

    if (renderer.has_font())
    {
        renderer.set_text_alignment(renderer::text_alignment_t::center);
        renderer.set_text_baseline(renderer::text_baseline_t::middle);
        renderer.render_text(action.name,
                             action.center_x,
                             action.center_y,
                             {255, 255, 255}, 5.0);
        renderer.set_text_alignment(renderer::text_alignment_t::left);
        renderer.set_text_baseline(renderer::text_baseline_t::top);
    }

    if (action.touched)
    {
        application::instance()
            .get_renderer()
            .set_blend_mode(snail::blend_mode_t::blend);
        application::instance()
            .get_renderer()
            .set_draw_color({192, 192, 192, 128});
        application::instance()
            .get_renderer()
            .fill_rect(x-(size/2), y-(size/2), size*2, size*2);
    }

    // TODO
    // application::instance()
    //     .get_renderer()
    //     .set_draw_color({192, 0, 192, 128});
    // application::instance()
    //     .get_renderer()
    //     .fill_rect(x, y, size, size);
}

void touch_input::on_touch_event(::SDL_TouchFingerEvent event)
{
    int norm_x = application::instance().physical_width() * event.x;
    int norm_y = application::instance().physical_height() * event.y;
    const constexpr int size = 300;
    last_touched_key_ = none;

    for (auto it = quick_actions_.begin(); it < quick_actions_.end(); it++)
    {
        quick_action& action = *it;

        if (last_touched_key_ == none
            && event.type != event_type::up
            && is_touched(norm_x, norm_y, action))
        {
            action.touched = true;
            last_touched_key_ = action.key;
        }
        else
        {
            action.touched = false;
        }
    }
}

}
}
