#include "particle.h"

Particle::Particle(float x, float y, float vx, float vy, float gravity, int color, int duration) :
  x_(x), y_(y), vx_(vx), vy_(vy), gravity_(gravity),
  color_(color),
  duration_(duration), timer_(0) {}

void Particle::update(unsigned int elapsed) {
  x_ += vx_ * elapsed;
  y_ += vy_ * elapsed;
  vy_ += gravity_ * elapsed;
  timer_ += elapsed;
}

void Particle::draw(Graphics& graphics) const {
  int a = 255 * (1 - timer_ / (float) duration_);
  graphics.draw_pixel(x_, y_, (color_ & 0xffffff00) | a);
}
