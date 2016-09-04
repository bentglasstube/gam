#include "text.h"

// TODO replace with spritemap

Text::Text(const std::string& file, int width) : file_(file), width_(width) {}

void Text::draw(Graphics& graphics, const std::string& text, int x, int y, Text::Alignment alignment) const {
  SDL_Rect source = { 0, 0, width_, width_ * 2};
  SDL_Rect dest = { x, y, width_, width_ * 2 };

  switch (alignment) {
    case Alignment::LEFT:
      break;

    case Alignment::CENTER:
      dest.x -= width_ / 2 * text.length();
      break;

    case Alignment::RIGHT:
      dest.x -= width_ * text.length();
      break;
  }

  for (std::string::const_iterator i = text.begin(); i != text.end(); ++i) {
    unsigned int n = 0;
    if ((*i) >= ' ' && (*i) <= '~') n = (*i) - ' ';

    source.x = width_ * (n % 16);
    source.y = width_ * 2 * (n / 16);

    graphics.blit(file_, &source, &dest);

    if ((*i) == '\n' && alignment == Alignment::LEFT) {
      dest.x = x;
      dest.y += width_ * 2;
    } else {
      dest.x += width_;
    }
  }
}
