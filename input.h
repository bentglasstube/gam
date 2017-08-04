#pragma once

#include <string>
#include <unordered_set>
#include <vector>
#include <SDL2/SDL.h>

class Input {
  public:

    enum class Button {
      None,
      Up, Down, Left, Right,
      Start, Select, A, B,
    };

    Input();
    ~Input();

    void begin_frame();
    void key_down(const SDL_Event& event);
    void key_up(const SDL_Event& event);
    void joy_down(const SDL_Event& event);
    void joy_up(const SDL_Event& event);
    void joy_axis(const SDL_Event& event);
    void joy_hat(const SDL_Event& event);

    bool key_pressed(Button key) const { return pressed_.count(key) > 0; }
    bool key_released(Button key) const { return released_.count(key) > 0; }
    bool key_held(Button key) const { return held_.count(key) > 0; }

    bool any_pressed() const { return !pressed_.empty(); }

    std::vector<Button> all_pressed() const;

    bool editting() const;
    void begin_editting();
    void end_editting();
    void text_input(const std::string& text);
    std::string get_string() const;

  private:

    static constexpr int kMaxAxes = 64;
    static constexpr int kDeadZone = 16000;

    class ButtonHash {
      public:
        size_t operator()(Button const& b) const;
    };
    typedef std::unordered_set<Button, ButtonHash> ButtonSet;

    ButtonSet held_, pressed_, released_;
    SDL_Joystick* joystick_;
    bool editting_;
    std::string string_;
    int axis_prev_[kMaxAxes], hat_prev_x_, hat_prev_y_;

    Button keybind(SDL_Scancode key);
    Button joybind(Uint8 button);

    void process_axis(int cur, int prev, Button neg, Button pos);
    void press(Button button);
    void release(Button button);
};
