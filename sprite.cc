#include "sprite.h"

Sprite::Sprite(const std::string& file, int x, int y, int w, int h) : file_(file) {
  rect_.x = x;
  rect_.y = y;
  rect_.w = w;
  rect_.h = h;
}

void Sprite::draw(Graphics& graphics, int x, int y) const {
  const Graphics::Rect dest = {x, y, rect_.w, rect_.h };
  graphics.blit(file_, rect_, dest);
}

void Sprite::draw_flip(Graphics& graphics, int x, int y, bool hflip, bool vflip) const {
  Graphics::Rect dest = { x, y, rect_.w, rect_.h };
  graphics.blit_flip(file_, rect_, dest, hflip, vflip);
}

void Sprite::draw_rot(Graphics& graphics, int x, int y, float angle, int cx, int cy) const {
  Graphics::Rect dest = { x, y, rect_.w, rect_.h };
  Graphics::Point center = { cx, cy };
  graphics.blit_rot(file_, rect_, dest, angle, center);
}
