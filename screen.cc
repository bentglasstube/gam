#include "screen.h"

Screen::Screen() : counter_(0) {
  for (int i = 0; i < kFpsFrames; ++i) ticks_[i] = 0;
}

Screen::~Screen() {}

float Screen::fps() const {
  unsigned int total = 0;
  for (int i = 0; i < kFpsFrames; ++i) total += ticks_[i];
  return kFpsFrames * 1000.0f / total;
}

void Screen::count_frame(unsigned int elapsed) {
  ticks_[counter_] = elapsed;
  counter_ = (counter_ + 1) % kFpsFrames;
}
