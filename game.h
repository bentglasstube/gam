#pragma once

#include <memory>

#include "audio.h"
#include "graphics.h"
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

  private:

    void init();

    Config config_;
    std::unique_ptr<Screen> screen_;
};
