#pragma once

#include <unordered_set>
#include <SDL2/SDL.h>

class Input {
  public:

    void begin_frame();
    void key_down(const SDL_Event& event);
    void key_up(const SDL_Event& event);

    bool key_pressed(SDL_Scancode key) const { return pressed_.count(key) > 0; }
    bool key_released(SDL_Scancode key) const { return released_.count(key) > 0; }
    bool key_held(SDL_Scancode key) const { return held_.count(key) > 0; }

    bool any_pressed() const { return !pressed_.empty(); }

  private:

    std::unordered_set<SDL_Scancode, std::hash<int>> held_;
    std::unordered_set<SDL_Scancode, std::hash<int>> pressed_;
    std::unordered_set<SDL_Scancode, std::hash<int>> released_;

};
