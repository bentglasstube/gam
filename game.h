#pragma once

#include <memory>

#include "audio.h"
#include "graphics.h"
#include "input.h"
#include "screen.h"

class Game {
  public:

    struct Config {
      Graphics::Config graphics;
      Audio::Config audio;
    };

    Game(const std::string title, int width, int height);
    Game(const Config& config);
    ~Game();

    void loop(Screen* initial_screen);

    void bind_key(SDL_Scancode scancode, Input::Button button);
    void bind_pad(SDL_GameControllerButton pad, Input::Button button);
    void bind_axis(int axis, Input::Button neg, Input::Button pos);

  private:

    void init();

    Config config_;
    Input input_;
    std::unique_ptr<Screen> screen_;
};
