#include "input.h"

void Input::begin_frame() {
  pressed_.clear();
  released_.clear();
}

void Input::key_down(const SDL_Event& event) {
  const Input::Button b = button_from_scancode(event.key.keysym.scancode);
  if (b != Input::Button::None) {
    pressed_.insert(b);
    held_.insert(b);
  }
}

void Input::key_up(const SDL_Event& event) {
  const Input::Button b = button_from_scancode(event.key.keysym.scancode);
  if (b != Input::Button::None) {
    released_.insert(b);
    held_.erase(b);
  }
}

std::vector<Input::Button> Input::all_pressed() const {
  std::vector<Input::Button> all(pressed_.size());
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

Input::Button Input::button_from_scancode(SDL_Scancode key) {
  switch (key) {
    case SDL_SCANCODE_W:
    case SDL_SCANCODE_UP:
      return Input::Button::Up;

    case SDL_SCANCODE_A:
    case SDL_SCANCODE_LEFT:
      return Input::Button::Left;

    case SDL_SCANCODE_S:
    case SDL_SCANCODE_DOWN:
      return Input::Button::Down;

    case SDL_SCANCODE_D:
    case SDL_SCANCODE_RIGHT:
      return Input::Button::Right;

    case SDL_SCANCODE_SPACE:
      return Input::Button::A;

    case SDL_SCANCODE_TAB:
      return Input::Button::Select;

    case SDL_SCANCODE_RETURN:
      return Input::Button::Start;

    default:
      return Input::Button::None;
  }
}

size_t Input::ButtonHash::operator()(Button const& b) const {
  return static_cast<int>(b);
}
