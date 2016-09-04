#include "backdrop.h"

Backdrop::Backdrop(const std::string& file) : file_(file) {}

void Backdrop::draw(Graphics& graphics) const {
  graphics.blit(file_, NULL, NULL);
}
