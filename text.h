#pragma once

#include <string>

#include "graphics.h"

class Text {
  public:

    enum class Alignment { Left, Center, Right };

    Text(const std::string& file, int width, int height);
    inline Text(const std::string& file, int width) : Text(file, width, width * 2) {}
    inline Text(const std::string& file) : Text(file, 8, 16) {}

    void draw(Graphics& graphics, const std::string& text, int x, int y, Alignment alignment=Alignment::Left) const;

  private:

    std::string file_;
    int width_, height_;
};

