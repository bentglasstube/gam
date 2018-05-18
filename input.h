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
      A, B, X, Y, L, R,
      User1, User2, User3, User4
    };

    Input();

    void init();
    bool process();

    bool has_gamepad() const { return gamepad_; };

    bool key_pressed(Button key) const { return pressed_.count(key) > 0; }
    bool key_released(Button key) const { return released_.count(key) > 0; }
    bool key_held(Button key) const { return held_.count(key) > 0; }
    bool any_pressed() const { return !pressed_.empty(); }
    std::vector<Button> all_pressed() const;

    bool editing() const;
    std::string get_string() const;

    void bind_key(SDL_Scancode scancode, Button button);
    void bind_pad(SDL_GameControllerButton pad, Button button);
    void bind_axis(int axis, Button neg, Button pos);

  private:

    static constexpr int kMaxAxes = 64;
    static constexpr int kDeadZone = 16000;

    static const std::unordered_map<int, Button> kDefaultKeyBinds;
    static const std::unordered_map<int, Button> kDefaultPadBinds;
    static const std::unordered_map<int, std::pair<Button, Button>> kDefaultAxisBinds;
    static const std::vector<std::string> kGamepadDB;

    typedef std::unordered_set<Button, Util::CastHash<Button>> ButtonSet;

    ButtonSet held_, pressed_, released_;
    bool editing_, gamepad_;
    std::string string_;
    int axis_prev_[kMaxAxes];
    std::unordered_map<int, Button> keybinds_, padbinds_;
    std::unordered_map<int, std::pair<Button, Button>> axisbinds_;

    Button keybind(SDL_Scancode key) const;
    Button padbind(SDL_GameControllerButton button) const;
    Button axisbind(int axis) const;

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

    void process_controllers();
};
