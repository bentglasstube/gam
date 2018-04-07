#include "input.h"

Input::Input() : gamepad_(nullptr) {

  SDL_GameControllerAddMapping("03000000571d00002100000010010000,Tomee NES Adapter,a:b0,b:b1,back:b2,start:b3,leftx:a0,lefty:a1");

  keybinds_ = kDefaultKeyBinds;
  padbinds_ = kDefaultPadBinds;
  axisbinds_ = kDefaultAxisBinds;

  const int count = SDL_NumJoysticks();
  for (int i = 0; i < count; ++i) {
    if (SDL_IsGameController(i)) {
      gamepad_ = SDL_GameControllerOpen(i);
      if (gamepad_) {
        fprintf(stderr, "Found controller %s\n", SDL_JoystickNameForIndex(i));
        break;
      }
    }
  }

  for (int i = 0; i < kMaxAxes; ++i) {
    axis_prev_[i] = 0;
  }
}

Input::~Input() {
  if (gamepad_) SDL_GameControllerClose(gamepad_);
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
        end_editing();
        break;

      case SDL_TEXTEDITING:
        text_input(event.edit.text);
        break;

      case SDL_CONTROLLERBUTTONDOWN:
        pad_down(event);
        break;

      case SDL_CONTROLLERBUTTONUP:
        pad_up(event);
        break;

      case SDL_CONTROLLERAXISMOTION:
        pad_axis(event);
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

bool Input::editing() const {
  return editing_;
}

std::string Input::get_string() const {
  return string_;
}

void Input::bind_key(SDL_Scancode scancode, Input::Button button) {
  keybinds_[scancode] = button;
}

void Input::bind_pad(SDL_GameControllerButton pad, Input::Button button) {
  padbinds_[pad] = button;
}

void Input::bind_axis(int axis, Input::Button neg, Input::Button pos) {
  axisbinds_[axis] = std::make_pair(neg, pos);
}

void Input::key_down(const SDL_Event& event) {
  press(keybind(event.key.keysym.scancode));
}

void Input::key_up(const SDL_Event& event) {
  release(keybind(event.key.keysym.scancode));
}

void Input::pad_down(const SDL_Event& event) {
  press(padbind(static_cast<SDL_GameControllerButton>(event.cbutton.button)));
}

void Input::pad_up(const SDL_Event& event) {
  release(padbind(static_cast<SDL_GameControllerButton>(event.cbutton.button)));
}

void Input::pad_axis(const SDL_Event& event) {
  if (event.caxis.axis >= kMaxAxes) return;

  int dir = 0;
  if (event.caxis.value < -kDeadZone) dir = -1;
  if (event.caxis.value > kDeadZone) dir = 1;

  const auto& i = axisbinds_.find(event.caxis.axis);
  if (i == axisbinds_.end()) return;

  const int prev = axis_prev_[event.caxis.axis];
  if (dir == prev) return;

  switch (dir) {
    case -1:
      press(i->second.first);
      if (prev == 1) release(i->second.second);
      break;

    case 0:
      if (prev == -1) release(i->second.first);
      if (prev == 1) release(i->second.second);
      break;

    case 1:
      if (prev == -1) release(i->second.first);
      press(i->second.second);
      break;
  }

  axis_prev_[event.caxis.axis] = dir;
}

void Input::begin_editing() {
  editing_ = true;
  string_ = "";
  SDL_StartTextInput();
}

void Input::end_editing() {
  editing_ = false;
  SDL_StopTextInput();
}

void Input::text_input(const std::string& text) {
  string_ = text;
}

Input::Button Input::keybind(SDL_Scancode key) const {
  const auto& i = keybinds_.find(key);
  return i == keybinds_.end() ? Input::Button::None : i->second;
}

Input::Button Input::padbind(SDL_GameControllerButton button) const {
  const auto& i = padbinds_.find(button);
  return i == padbinds_.end() ? Input::Button::None : i->second;
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

const std::unordered_map<int, Input::Button> Input::kDefaultKeyBinds = {
  { SDL_SCANCODE_UP,      Input::Button::Up },
  { SDL_SCANCODE_W,       Input::Button::Up },
  { SDL_SCANCODE_A,       Input::Button::Left },
  { SDL_SCANCODE_LEFT,    Input::Button::Left },
  { SDL_SCANCODE_S,       Input::Button::Down },
  { SDL_SCANCODE_DOWN,    Input::Button::Down },
  { SDL_SCANCODE_D,       Input::Button::Right },
  { SDL_SCANCODE_RIGHT,   Input::Button::Right },
  { SDL_SCANCODE_SPACE,   Input::Button::A },
  { SDL_SCANCODE_J,       Input::Button::B },
  { SDL_SCANCODE_K,       Input::Button::X },
  { SDL_SCANCODE_L,       Input::Button::Y },
  { SDL_SCANCODE_Z,       Input::Button::A },
  { SDL_SCANCODE_X,       Input::Button::B },
  { SDL_SCANCODE_C,       Input::Button::X },
  { SDL_SCANCODE_V,       Input::Button::Y },
  { SDL_SCANCODE_LSHIFT,  Input::Button::L },
  { SDL_SCANCODE_RSHIFT,  Input::Button::R },
  { SDL_SCANCODE_TAB,     Input::Button::Select },
  { SDL_SCANCODE_RETURN,  Input::Button::Start },
};

const std::unordered_map<int, Input::Button> Input::kDefaultPadBinds = {
  { SDL_CONTROLLER_BUTTON_DPAD_UP,       Input::Button::Up },
  { SDL_CONTROLLER_BUTTON_DPAD_LEFT,     Input::Button::Left },
  { SDL_CONTROLLER_BUTTON_DPAD_DOWN,     Input::Button::Down },
  { SDL_CONTROLLER_BUTTON_DPAD_RIGHT,    Input::Button::Right },
  { SDL_CONTROLLER_BUTTON_A,             Input::Button::A },
  { SDL_CONTROLLER_BUTTON_B,             Input::Button::B },
  { SDL_CONTROLLER_BUTTON_X,             Input::Button::X },
  { SDL_CONTROLLER_BUTTON_Y,             Input::Button::Y },
  { SDL_CONTROLLER_BUTTON_LEFTSHOULDER,  Input::Button::L },
  { SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, Input::Button::R },
  { SDL_CONTROLLER_BUTTON_BACK,          Input::Button::Select },
  { SDL_CONTROLLER_BUTTON_START,         Input::Button::Start },
};

const std::unordered_map<int, std::pair<Input::Button, Input::Button>> Input::kDefaultAxisBinds = {
  { 0, { Input::Button::Left, Input::Button::Right } },
  { 1, { Input::Button::Up, Input::Button::Down } },
  { 2, { Input::Button::Left, Input::Button::Right } },
  { 3, { Input::Button::Up, Input::Button::Down } },
};
