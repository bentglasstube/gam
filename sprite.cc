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
