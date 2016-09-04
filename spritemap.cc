#include "spritemap.h"

SpriteMap::SpriteMap(const std::string& file, int cols, int width, int height) :
  file_(file), cols_(cols), width_(width), height_(height) {}

void SpriteMap::draw(Graphics& graphics, int n, int x, int y) const {
  SDL_Rect source = { 0, 0, width_, height_ };
  SDL_Rect dest = { x, y, width_, height_ };

  source.x = width_ * (n % cols_);
  source.y = height_ * (n / cols_);

  graphics.blit(file_, &source, &dest);
}

void SpriteMap::draw_ex(Graphics& graphics, int n, int x, int y, bool flip, float angle, int cx, int cy) const {
  SDL_Rect source = { 0, 0, width_, height_ };
  SDL_Rect dest = { x, y, width_, height_ };
  SDL_Point center = { cx, cy };

  source.x = width_ * (n % cols_);
  source.y = height_ * (n / cols_);

  graphics.blit_ex(file_, &source, &dest, angle, &center, flip ? Graphics::FlipDirection::HORIZONTAL : Graphics::FlipDirection::NONE);
}
