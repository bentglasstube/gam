#pragma once

#include "graphics.h"

class SpriteMap {
  public:

    SpriteMap(const std::string& file, int cols, int width, int height);
    void draw(Graphics& graphics, int n, int x, int y) const;
    void draw_ex(Graphics& graphics, int n, int x, int y, bool flip, float angle, int cx, int cy) const;

  private:

    std::string file_;
    int cols_, width_, height_;

};
