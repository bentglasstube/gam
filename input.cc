#include "input.h"

void Input::begin_frame() {
  pressed_.clear();
  released_.clear();
}

void Input::key_down(const SDL_Event& event) {
  pressed_.insert(event.key.keysym.scancode);
  held_.insert(event.key.keysym.scancode);
}

void Input::key_up(const SDL_Event& event) {
  released_.insert(event.key.keysym.scancode);
  held_.erase(event.key.keysym.scancode);
}
