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

bool Input::process() {
  SDL_Event event;

  pressed_.clear();
  released_.clear();

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_KEYDOWN:
        if (event.key.repeat == 0) key_down(event);
        break;

      case SDL_KEYUP:
        key_up(event);
        break;

      case SDL_TEXTINPUT:
        text_input(event.text.text);
        end_editting();
        break;

      case SDL_TEXTEDITING:
        text_input(event.edit.text);
        break;

      case SDL_JOYBUTTONDOWN:
        joy_down(event);
        break;

      case SDL_JOYBUTTONUP:
        joy_up(event);
        break;

      case SDL_JOYAXISMOTION:
        joy_axis(event);
        break;

      case SDL_JOYHATMOTION:
        joy_hat(event);
        break;

      case SDL_QUIT:
        return false;
    }
  }

  return true;

}

std::vector<Input::Button> Input::all_pressed() const {
  std::vector<Input::Button> all(pressed_.size());
  std::copy(pressed_.begin(), pressed_.end(), all.begin());
  return all;
}

bool Input::editting() const {
  return editting_;
}

std::string Input::get_string() const {
  return string_;
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

Input::Button Input::keybind(SDL_Scancode key) const {
  const auto& i = kDefaultKeyBinds.find(key);
  return i == kDefaultKeyBinds.end() ? Input::Button::None : i->second;
}

Input::Button Input::joybind(Uint8 button) const {
  const auto& i = kDefaultJoyBinds.find(button);
  return i == kDefaultJoyBinds.end() ? Input::Button::None : i->second;
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

const std::unordered_map<SDL_Scancode, Input::Button> Input::kDefaultKeyBinds = {
  { SDL_SCANCODE_UP,      Input::Button::Up },
  { SDL_SCANCODE_W,       Input::Button::Up },
  { SDL_SCANCODE_A,       Input::Button::Left },
  { SDL_SCANCODE_LEFT,    Input::Button::Left },
  { SDL_SCANCODE_S,       Input::Button::Down },
  { SDL_SCANCODE_DOWN,    Input::Button::Down },
  { SDL_SCANCODE_D,       Input::Button::Right },
  { SDL_SCANCODE_RIGHT,   Input::Button::Right },
  { SDL_SCANCODE_J,       Input::Button::A },
  { SDL_SCANCODE_Z,       Input::Button::A },
  { SDL_SCANCODE_SPACE,   Input::Button::A },
  { SDL_SCANCODE_K,       Input::Button::B },
  { SDL_SCANCODE_X,       Input::Button::B },
  { SDL_SCANCODE_TAB,     Input::Button::Select },
  { SDL_SCANCODE_RETURN,  Input::Button::Start },
};

const std::unordered_map<Uint8, Input::Button> Input::kDefaultJoyBinds = {
  { 0, Input::Button::A },
  { 1, Input::Button::B },
  { 2, Input::Button::B },
  { 3, Input::Button::A },
  { 4, Input::Button::A },
  { 5, Input::Button::B },
  { 6, Input::Button::Select },
  { 6, Input::Button::Start },
};
