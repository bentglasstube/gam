#include "spritemap.h"

SpriteMap::SpriteMap(const std::string& file, int cols, int width, int height) :
  file_(file), cols_(cols), width_(width), height_(height) {}

void SpriteMap::draw(Graphics& graphics, int n, int x, int y) const {
  const Graphics::Rect source = { width_ * (n % cols_), height_ * (n / cols_), width_, height_ };
  const Graphics::Rect dest = { x, y, width_, height_ };
  graphics.blit(file_, source, dest);
}

void SpriteMap::draw_flip(Graphics& graphics, int n, int x, int y, bool hflip, bool vflip) const {
  const Graphics::Rect source = { width_ * (n % cols_), height_ * (n / cols_), width_, height_ };
  const Graphics::Rect dest = { x, y, width_, height_ };
  graphics.blit_flip(file_, source, dest, hflip, vflip);
}

void SpriteMap::draw_rot(Graphics& graphics, int n, int x, int y, float angle, int cx, int cy) const {
  const Graphics::Rect source = { width_ * (n % cols_), height_ * (n / cols_), width_, height_ };
  const Graphics::Rect dest = { x, y, width_, height_ };
  const Graphics::Point center = { cx, cy };
  graphics.blit_rot(file_, source, dest, angle, center);
}
