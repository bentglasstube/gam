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

std::vector<SDL_Scancode> Input::all_pressed() const {
  std::vector<SDL_Scancode> all(pressed_.size());
  std::copy(pressed_.begin(), pressed_.end(), all.begin());
  return all;
}

bool Input::editting() const {
  return editting_;
}

void Input::begin_editting() {
  editting_ = true;
  string_ = "";
  SDL_StartTextInput();
}

void Input::end_editting() {
  editting_ = false;
  SDL_StopTextInput();
}

void Input::text_input(const std::string& text) {
  string_ = text;
}

std::string Input::get_string() const {
  return string_;
}
