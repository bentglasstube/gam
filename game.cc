#include "game.h"

#include <SDL2/SDL.h>

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

  unsigned int last_update = SDL_GetTicks();

  screen_.reset(std::move(initial_screen));
  screen_->init();

  while (true) {
    if (!audio.music_playing()) audio.play_music(screen_->get_music_track());
    if (!input_->process()) return;

    const unsigned int update = SDL_GetTicks();
    const unsigned int frame_ticks = update - last_update;

    screen_->count_frame(frame_ticks);
    if (screen_->update(*(input_.get()), audio, frame_ticks)) {

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

  input_.reset();
}

void Game::bind_key(SDL_Scancode scancode, Input::Button button) {
  input_->bind_key(scancode, button);
}

void Game::bind_pad(SDL_GameControllerButton pad, Input::Button button) {
  input_->bind_pad(pad, button);
}

void Game::bind_axis(int axis, Input::Button neg, Input::Button pos) {
  input_->bind_axis(axis, neg, pos);
}

void Game::init() {
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");
  input_.reset(new Input());
}
