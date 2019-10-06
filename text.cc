#include "text.h"

// TODO replace with spritemap

Text::Text(const std::string& file, int width, int height) : file_(file), width_(width), height_(height) {}

void Text::draw(Graphics& graphics, const std::string& text, int x, int y, Text::Alignment alignment) const {
  SDL_Rect source = { 0, 0, width_, height_ };
  SDL_Rect dest = { x, y, width_, height_ };

  switch (alignment) {
    case Alignment::Left:
      break;

    case Alignment::Center:
      dest.x -= width_ / 2 * text.length();
      break;

    case Alignment::Right:
      dest.x -= width_ * text.length();
      break;
  }

  for (std::string::const_iterator i = text.begin(); i != text.end(); ++i) {
    unsigned int n = 0;
    if ((*i) >= ' ' && (*i) <= '~') n = (*i) - ' ';

    source.x = width_ * (n % 16);
    source.y = height_ * (n / 16);

    graphics.blit(file_, &source, &dest);

    if ((*i) == '\n' && alignment == Alignment::Left) {
      dest.x = x;
      dest.y += height_;
    } else {
      dest.x += width_;
    }
  }
}
