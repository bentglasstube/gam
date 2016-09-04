#include "graphics.h"

#include "math.h"

Graphics::Graphics(int width, int height) : width_(width), height_(height) {
  int flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_FULLSCREEN_DESKTOP;

  window_ = SDL_CreateWindow("Catapults", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width_, height_, flags);
  renderer_ = SDL_CreateRenderer(window_, -1, 0);

  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
  SDL_RenderSetLogicalSize(renderer_, width_, height_);
  SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND);
}

Graphics::~Graphics() {
  for (TextureMap::iterator i = textures_.begin(); i != textures_.end(); ++i) {
    SDL_DestroyTexture(i->second);
  }

  SDL_DestroyRenderer(renderer_);
  SDL_DestroyWindow(window_);
}

void Graphics::blit(const std::string& file, const SDL_Rect* srect, const SDL_Rect* drect) {
  SDL_Texture* texture = load_image(file);
  SDL_RenderCopy(renderer_, texture, srect, drect);
}

void Graphics::blit_ex(const std::string& file, const SDL_Rect* srect, const SDL_Rect* drect, const float angle, const SDL_Point* center, const Graphics::FlipDirection flip) {
  SDL_RendererFlip f = SDL_FLIP_NONE;
  switch (flip) {
    case FlipDirection::NONE:
      f = SDL_FLIP_NONE;
      break;

    case FlipDirection::HORIZONTAL:
      f = SDL_FLIP_HORIZONTAL;
      break;

    case FlipDirection::VERTICAL:
      f = SDL_FLIP_VERTICAL;
      break;

    case FlipDirection::BOTH:
      f = (SDL_RendererFlip) (SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
      break;
  }

  SDL_Texture* texture = load_image(file);
  SDL_RenderCopyEx(renderer_, texture, srect, drect, angle * 180.0f / M_PI, center, f);
}

void Graphics::flip() {
  SDL_RenderPresent(renderer_);
}

void Graphics::clear() {
  SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
  SDL_RenderClear(renderer_);
}

void Graphics::draw_pixel(int x, int y, int color) {
  set_color(color);
  SDL_RenderDrawPoint(renderer_, x, y);
}

void Graphics::draw_line(int x1, int y1, int x2, int y2, int color) {
  set_color(color);
  SDL_RenderDrawLine(renderer_, x1, y1, x2, y2);
}

void Graphics::draw_rect(const SDL_Rect* rect, int color, bool filled) {
  set_color(color);
  filled ? SDL_RenderFillRect(renderer_, rect) : SDL_RenderDrawRect(renderer_, rect);
}

void Graphics::draw_circle(int x, int y, int r, int color, bool filled) {
  set_color(color);
  int cx = r, cy = 0, error = 0;

  while (cx >= cy) {
    if (filled) {
      SDL_RenderDrawLine(renderer_, x - cx, y + cy, x + cx, y + cy);
      SDL_RenderDrawLine(renderer_, x - cy, y + cx, x + cy, y + cx);
      SDL_RenderDrawLine(renderer_, x - cx, y - cy, x + cx, y - cy);
      SDL_RenderDrawLine(renderer_, x - cy, y - cx, x + cy, y - cx);
    } else {
      SDL_Point points[8];

      points[0].x = points[7].x = x + cx;
      points[1].x = points[6].x = x + cy;
      points[2].x = points[5].x = x - cy;
      points[3].x = points[4].x = x - cx;

      points[0].y = points[3].y = y + cy;
      points[1].y = points[2].y = y + cx;
      points[4].y = points[7].y = y - cy;
      points[5].y = points[6].y = y - cx;

      SDL_RenderDrawPoints(renderer_, points, 8);
    }

    ++cy;
    error += 1 + 2 * cy;
    if (2 * (error - cx) + 1 > 0) {
      --cx;
      error += 1 - 2 * cx;
    }
  }
}

SDL_Texture* Graphics::load_image(const std::string& file) {
  const std::string path("content/" + file+ ".bmp");
  if (textures_.count(path) == 0) {
    SDL_Surface* surface = SDL_LoadBMP(path.c_str());
    const Uint32 black = SDL_MapRGB(surface->format, 0, 0, 0);
    SDL_SetColorKey(surface, SDL_TRUE, black);

    textures_[path] = SDL_CreateTextureFromSurface(renderer_, surface);
  }

  return textures_[path];
}

void Graphics::set_color(int color) {
  const int r = (color & 0xff000000) >> 24;
  const int g = (color & 0x00ff0000) >> 16;
  const int b = (color & 0x0000ff00) >> 8;
  const int a = (color & 0x000000ff);

  SDL_SetRenderDrawColor(renderer_, r, g, b, a);
}
