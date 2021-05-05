#pragma once

#include "graphics.h"

class Box {

  public:

    Box(const std::string& file, int width, int height);
    void draw(Graphics& graphics, int x, int y, int w, int h) const;

  private:

    std::string file_;
    int width_, height_;

};
