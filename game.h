#pragma once

#include <memory>

#include "screen.h"

class Game {
  public:

    Game(int width, int height);
    ~Game();

    void loop(Screen* initial_screen);

  private:

    int w_, h_;
    std::shared_ptr<Screen> screen_;
};
