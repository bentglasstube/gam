#include "sprite.h"

Sprite::Sprite(const std::string& file, int x, int y, int w, int h) : file_(file) {
  rect_.x = x;
  rect_.y = y;
  rect_.w = w;
  rect_.h = h;
}

void Sprite::draw(Graphics& graphics, int x, int y) const {
  const SDL_Rect dest = {x, y, rect_.w, rect_.h };
  graphics.blit(file_, &rect_, &dest);
}

void Sprite::draw_ex(Graphics& graphics, int x, int y, bool flip, float angle, int cx, int cy) const {
  SDL_Rect dest = { x, y, rect_.w, rect_.h };
  SDL_Point center = { cx, cy };

  graphics.blit_ex(file_, &rect_, &dest, angle, &center, flip ? Graphics::FlipDirection::HORIZONTAL : Graphics::FlipDirection::NONE);
}
