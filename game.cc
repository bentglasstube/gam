#include "game.h"

#include <SDL2/SDL.h>

#include "input.h"

Game::Game(const std::string title, int width, int height) {
  config_.graphics.title = title;
  config_.graphics.width = width;
  config_.graphics.height = height;

  init();
}

Game::Game(const Config& config) : config_(config) {
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

  screen_.reset(std::move(initial_screen));
  screen_->init();

  while (true) {
    if (!audio.music_playing()) audio.play_music(screen_->get_music_track());
    if (!input.process()) return;

    const unsigned int update = SDL_GetTicks();
    const unsigned int frame_ticks = update - last_update;

    screen_->count_frame(frame_ticks);
    if (screen_->update(input, audio, frame_ticks)) {

      graphics.clear();
      screen_->draw(graphics);
      graphics.flip();

    } else {

      screen_.reset(std::move(screen_->next_screen()));
      if (!screen_) return;
      screen_->init();

      audio.stop_music();
    }

    last_update = update;
  }
}

void Game::init() {
  SDL_Init(SDL_INIT_EVERYTHING);
}
