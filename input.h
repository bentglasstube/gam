#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <SDL2/SDL.h>

#include "util.h"

class Input {
  public:

    enum class Button {
      None,
      Up, Down, Left, Right,
      Start, Select,
      A, B, X, Y, L, R
    };

    Input();
    ~Input();

    bool process();

    bool key_pressed(Button key) const { return pressed_.count(key) > 0; }
    bool key_released(Button key) const { return released_.count(key) > 0; }
    bool key_held(Button key) const { return held_.count(key) > 0; }
    bool any_pressed() const { return !pressed_.empty(); }
    std::vector<Button> all_pressed() const;
    bool editing() const;
    std::string get_string() const;

  private:

    static constexpr int kMaxAxes = 64;
    static constexpr int kDeadZone = 16000;

    static const std::unordered_map<int, Button> kDefaultKeyBinds;
    static const std::unordered_map<int, Button> kDefaultPadBinds;

    typedef std::unordered_set<Button, Util::CastHash<Button>> ButtonSet;

    ButtonSet held_, pressed_, released_;
    SDL_GameController* gamepad_;
    bool editing_;
    std::string string_;
    int axis_prev_[kMaxAxes];

    Button keybind(SDL_Scancode key) const;
    Button padbind(SDL_GameControllerButton button) const;

    void key_down(const SDL_Event& event);
    void key_up(const SDL_Event& event);
    void pad_down(const SDL_Event& event);
    void pad_up(const SDL_Event& event);
    void pad_axis(const SDL_Event& event);

    void begin_editing();
    void end_editing();
    void text_input(const std::string& text);

    void process_axis(int cur, int prev, Button neg, Button pos);
    void press(Button button);
    void release(Button button);
};
