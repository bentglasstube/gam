#include "input.h"

Input::Input() : joystick_(nullptr), hat_prev_x_(0), hat_prev_y_(0) {
  for (int i = 0; i < SDL_NumJoysticks(); ++i) {
    joystick_ = SDL_JoystickOpen(i);
    break;
  }

  for (int i = 0; i < kMaxAxes; ++i) {
    axis_prev_[i] = 0;
  }
}

Input::~Input() {
  if (joystick_) {
    SDL_JoystickClose(joystick_);
  }
}

void Input::begin_frame() {
  pressed_.clear();
  released_.clear();
}

void Input::key_down(const SDL_Event& event) {
  press(keybind(event.key.keysym.scancode));
}

void Input::key_up(const SDL_Event& event) {
  release(keybind(event.key.keysym.scancode));
}

void Input::joy_up(const SDL_Event& event) {
  press(joybind(event.jbutton.button));
}

void Input::joy_down(const SDL_Event& event) {
  release(joybind(event.jbutton.button));
}

void Input::joy_axis(const SDL_Event& event) {
  if (event.jaxis.axis >= kMaxAxes) return;

  int dir = 0;
  if (event.jaxis.value < -kDeadZone) dir = -1;
  if (event.jaxis.value > kDeadZone) dir = 1;

  Input::Button neg, pos;

  switch (event.jaxis.axis) {
    case 0:
    case 2:
      neg = Input::Button::Left;
      pos = Input::Button::Right;
      break;

    case 1:
    case 3:
      neg = Input::Button::Up;
      pos = Input::Button::Down;
      break;

    default:
      neg = pos = Input::Button::None;
      break;
  }

  process_axis(dir, axis_prev_[event.jaxis.axis], neg, pos);
  axis_prev_[event.jaxis.axis] = dir;
}

void Input::joy_hat(const SDL_Event& event) {
  int x = 0, y = 0;

  switch (event.jhat.value) {
    case SDL_HAT_LEFTUP:    x = -1; y = -1; break;
    case SDL_HAT_LEFT:      x = -1; y =  0; break;
    case SDL_HAT_LEFTDOWN:  x = -1; y =  1; break;
    case SDL_HAT_UP:        x =  0; y = -1; break;
    case SDL_HAT_CENTERED:  x =  0; y =  0; break;
    case SDL_HAT_DOWN:      x =  0; y =  1; break;
    case SDL_HAT_RIGHTUP:   x =  1; y = -1; break;
    case SDL_HAT_RIGHT:     x =  1; y =  0; break;
    case SDL_HAT_RIGHTDOWN: x =  1; y =  1; break;
  }

  process_axis(x, hat_prev_x_, Input::Button::Left, Input::Button::Right);
  process_axis(y, hat_prev_y_, Input::Button::Up, Input::Button::Down);

  hat_prev_x_ = x;
  hat_prev_y_ = y;
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

Input::Button Input::keybind(SDL_Scancode key) {
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

    case SDL_SCANCODE_J:
    case SDL_SCANCODE_Z:
    case SDL_SCANCODE_SPACE:
      return Input::Button::A;

    case SDL_SCANCODE_K:
    case SDL_SCANCODE_X:
      return Input::Button::B;

    case SDL_SCANCODE_TAB:
      return Input::Button::Select;

    case SDL_SCANCODE_RETURN:
      return Input::Button::Start;

    default:
      return Input::Button::None;
  }
}

Input::Button Input::joybind(Uint8 button) {
  switch (button) {
    case 0:
    case 3:
    case 4:
      return Input::Button::A;

    case 1:
    case 2:
    case 5:
      return Input::Button::B;

    case 6:
      return Input::Button::Select;

    case 7:
      return Input::Button::Start;

    default:
      return Input::Button::None;
  }
}

size_t Input::ButtonHash::operator()(Button const& b) const {
  return static_cast<size_t>(b);
}

void Input::process_axis(int cur, int prev, Button neg, Button pos) {
  if (cur == prev) return;

  switch (cur) {
    case -1:
      press(neg);
      if (prev == 1) release(pos);
      break;

    case 0:
      if (prev == -1) release(neg);
      if (prev == 0) release(pos);
      break;

    case 1:
      if (prev == -1) release(neg);
      press(pos);
      break;
  }
}

void Input::press(Input::Button b) {
  if (b != Input::Button::None) {
    pressed_.insert(b);
    held_.insert(b);
  }
}

void Input::release(Input::Button b) {
  if (b != Input::Button::None) {
    released_.insert(b);
    held_.erase(b);
  }
}
