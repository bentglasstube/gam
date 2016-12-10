#include "font.h"

Font::Font(const std::string& file, int size) {
  if (!TTF_WasInit()) TTF_Init();

  const std::string path("content/" + file);
  font_ = TTF_OpenFont(path.c_str(), size);

  if (font_ == NULL) {
    fprintf(stderr, "Unable to open font %s: %s\n", path.c_str(), TTF_GetError());
  }
}

Font::~Font() {
  TTF_CloseFont(font_);
}

void Font::draw(Graphics& graphics, const std::string& text, int x, int y, int color) {
  SDL_Color c;

  c.r = (color & 0xff000000) >> 24;
  c.g = (color & 0x00ff0000) >> 16;
  c.b = (color & 0x0000ff00) >> 8;
  c.a = (color & 0x000000ff);

  SDL_Surface* surface = TTF_RenderText_Solid(font_, text.c_str(), c);
  graphics.draw_surface(surface, x, y);
}
