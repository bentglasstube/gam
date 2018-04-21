#pragma once

#include <string>

#include "graphics.h"

class ParallaxBackdrop {
  public:
    ParallaxBackdrop(const std::string& file, int width, int height, double scale);

    void draw(Graphics& graphics, double xoffset, double yoffset) const;

  private:

    std::string file_;
    int width_, height_;
    double scale_;
};
