#include "screen.h"

Screen::Screen() : counter_(0) {
  for (int i = 0; i < FPS_FRAMES; ++i) ticks_[i] = 0;
}

bool Screen::process_input(Input& input) {
  SDL_Event event;

  input.begin_frame();

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_KEYDOWN:
        if (event.key.repeat == 0) input.key_down(event);
        break;

      case SDL_KEYUP:
        input.key_up(event);
        break;

      case SDL_TEXTINPUT:
        input.text_input(event.text.text);
        input.end_editting();
        break;

      case SDL_TEXTEDITING:
        input.text_input(event.edit.text);
        break;

      case SDL_QUIT:
        return false;
    }
  }

  return true;
}

float Screen::fps() const {
  unsigned int total = 0;
  for (int i = 0; i < FPS_FRAMES; ++i) total += ticks_[i];
  return FPS_FRAMES * 1000.0f / total;
}

void Screen::count_frame(unsigned int elapsed) {
  ticks_[counter_] = elapsed;
  counter_ = (counter_ + 1) % FPS_FRAMES;
}
