#include "screen.h"

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

      case SDL_QUIT:
        return false;
    }
  }

  return true;
}
