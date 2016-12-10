#pragma once

#include <SDL2/SDL_ttf.h>
#include <string>

#include "graphics.h"

class Font {
  public:

    Font(const std::string& file, int size = 12);
    ~Font();

    void draw(Graphics& graphics, const std::string& text, int x, int y, int color);

  private:

    TTF_Font* font_;
};
