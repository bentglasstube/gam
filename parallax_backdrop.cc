#include "parallax_backdrop.h"

ParallaxBackdrop::ParallaxBackdrop(const std::string& file, int width, int height, double scale) :
  file_(file), width_(width), height_(height), scale_(scale) {}

void ParallaxBackdrop::draw(Graphics& graphics, double xoffset, double) const {
  const Graphics::Rect src = { 0, 0, width_, height_ };
  Graphics::Rect dest = { 0, 0, width_, height_ };

  dest.x = -xoffset / scale_;
  while (dest.x < graphics.width()) {
    graphics.blit(file_, src, dest);
    dest.x += width_;
  }
}
