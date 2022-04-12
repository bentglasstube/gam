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
  start(initial_screen);
  while (step()) {
    // do nothing
  }
}

void Game::start(Screen* initial_screen) {
  graphics_.reset(new Graphics(config_.graphics));
  audio_.reset(new Audio(config_.audio));

  last_update_ = SDL_GetTicks();

  set_screen(std::move(initial_screen));
}

bool Game::step() {
  if (!input_.process()) return false;

  const unsigned int update = SDL_GetTicks();
  const unsigned int frame_ticks = update - last_update_;

  if (input_.key_pressed(Input::Button::FullScreen))
    graphics_->toggle_fullscreen();

  screen_->count_frame(frame_ticks);
  if (screen_->update(input_, static_cast<Audio&>(*audio_), frame_ticks)) {
    graphics_->clear();
    screen_->draw(static_cast<Graphics&>(*graphics_));
    graphics_->flip();
  } else {
    set_screen(std::move(screen_->next_screen()));
  }

  last_update_ = update;
  return screen_.get() != nullptr;
}

void Game::bind_key(SDL_Scancode scancode, Input::Button button) {
  input_.bind_key(scancode, button);
}

void Game::bind_pad(SDL_GameControllerButton pad, Input::Button button) {
  input_.bind_pad(pad, button);
}

void Game::bind_axis(int axis, Input::Button neg, Input::Button pos) {
  input_.bind_axis(axis, neg, pos);
}

void Game::init() {
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");
  input_.init();
}

void Game::set_screen(Screen* screen) {
  screen_.reset(std::move(screen));

  if (screen_) {
    const std::string track = screen_->get_music_track();
    if (track == "") {
      audio_->stop_music();
    } else if (track != music_track_) {
      music_track_ = track;
      audio_->play_music(track, true);
    }

    screen_->loader(static_cast<Graphics&>(*graphics_));
  }
}
