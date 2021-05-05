#pragma once

#include "graphics.h"

class SpriteMap {
  public:

    SpriteMap(const std::string& file, int cols, int width, int height);
    void draw(Graphics& graphics, int n, int x, int y) const;
    void draw_flip(Graphics& graphics, int n, int x, int y, bool hflip, bool vflip) const;
    void draw_rot(Graphics& graphics, int n, int x, int y, float angle, int cx, int cy) const;

  private:

    std::string file_;
    int cols_, width_, height_;

};
