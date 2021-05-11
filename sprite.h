#pragma once

#include <string>

#include "graphics.h"

class Sprite {
  public:

    Sprite(const std::string& file, int x, int y, int w, int h);

    virtual void draw(Graphics& graphics, int x, int y) const;
    virtual void draw_flip(Graphics& graphics, int x, int y, bool hflip, bool vflip) const;
    virtual void draw_rot(Graphics& graphics, int x, int y, float angle, int cx, int cy) const;

  protected:
    Graphics::Rect rect_;

  private:
    std::string file_;
};
