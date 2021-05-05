#include "box.h"

Box::Box(const std::string& file, int width, int height) :
  file_(file), width_(width), height_(height) {}

void Box::draw(Graphics& graphics, int x, int y, int w, int h) const {
  const int ex = x + w - width_;
  const int ey = y + h - height_;

  // corners
  graphics.blit(file_, {0,          0,           width_, height_}, { x,  y, width_, height_});
  graphics.blit(file_, {2 * width_, 0,           width_, height_}, {ex,  y, width_, height_});
  graphics.blit(file_, {0,          2 * height_, width_, height_}, { x, ey, width_, height_});
  graphics.blit(file_, {2 * width_, 2 * height_, width_, height_}, {ex, ey, width_, height_});

  // sides
  graphics.blit(file_, {width_,     0,           width_, height_}, {x + width_, y,           w - 2 * width_, height_});
  graphics.blit(file_, {width_,     2 * height_, width_, height_}, {x + width_, ey,          w - 2 * width_, height_});
  graphics.blit(file_, {0,          height_,     width_, height_}, {x,          y + height_, width_, h - 2 * height_});
  graphics.blit(file_, {2 * width_, height_,     width_, height_}, {ex,         y + height_, width_, h - 2 * height_});

  // middle
  graphics.blit(file_, {width_, height_, width_, height_}, {x + width_, y + height_, w - 2 * width_, h - 2 * height_});
}
