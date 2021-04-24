#include "rect.h"

#include <cassert>

Rect::Rect() : Rect(0, 0, 0, 0) {}
Rect::Rect(double t, double l, double r, double b) : top(t), left(l), right(r), bottom(b) {
  assert(t <= b);
  assert(l <= r);
}

bool Rect::intersect(const Rect& other) const {
  return left < other.right && right > other.left && top < other.bottom && bottom > other.top;
}

bool Rect::contains(double x, double y) const {
  return left <= x && right >= x && top <= y && bottom >= y;
}

void Rect::draw(Graphics& graphics, int xo, int yo, Graphics::Color color, bool filled) const {
  const Graphics::Point p1 = { (int)(left - xo), (int)(top - yo) };
  const Graphics::Point p2 = { (int)(right - xo), (int)(bottom - yo) };
  graphics.draw_rect(p1, p2, color, filled);
}
