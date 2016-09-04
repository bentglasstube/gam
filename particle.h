#pragma once

#include "graphics.h"

class Particle {
  public:
    Particle(float x, float y, float vx, float vy, float gravity, int color, int duration);

    void update(unsigned int elapsed);
    void draw(Graphics& graphics) const;

    bool done() const { return timer_ > duration_; }

  private:

    float x_, y_, vx_, vy_, gravity_;
    int color_, duration_, timer_;
};
