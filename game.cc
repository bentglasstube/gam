#include "game.h"

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>

#include "audio.h"
#include "graphics.h"
#include "input.h"

Game::Game(int width, int height) : w_(width), h_(height) {
  srand(static_cast<unsigned int>(time(NULL)));
  SDL_Init(SDL_INIT_EVERYTHING);
}

Game::~Game() {
  SDL_Quit();
}

void Game::loop(Screen* initial_screen) {
  Graphics graphics(w_, h_);
  Audio audio;
  Input input;

  unsigned int last_update = SDL_GetTicks();

  screen_.reset(initial_screen);
  screen_->init();

  while (true) {
    if (!audio.music_playing()) audio.play_music(screen_->get_music_track());
    if (!screen_->process_input(input)) return;

    const unsigned int update = SDL_GetTicks();
    const unsigned int frame_ticks = update - last_update;
    if (screen_->update(input, audio, frame_ticks)) {

      graphics.clear();
      screen_->draw(graphics);
      graphics.flip();

    } else {

      screen_.reset(screen_->next_screen());
      if (!screen_) return;
      screen_->init();

      audio.stop_music();
    }

    last_update = update;
  }
}
