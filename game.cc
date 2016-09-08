#include "game.h"

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>

#include "input.h"

Game::Game(const std::string title, int width, int height) {
  config_.graphics.title = title;
  config_.graphics.width = width;
  config_.graphics.height = height;

  init();
}

Game::Game(const Game::Config config) : config_(config) {
  init();
}

Game::~Game() {
  SDL_Quit();
}

void Game::loop(Screen* initial_screen) {
  Graphics graphics(config_.graphics);
  Audio audio(config_.audio);
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

void Game::init() {
  srand(static_cast<unsigned int>(time(NULL)));
  SDL_Init(SDL_INIT_EVERYTHING);
}
