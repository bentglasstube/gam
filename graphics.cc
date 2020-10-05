#include "graphics.h"

#include <assert.h>
#include <cmath>
#include <utility>

#include <SDL2/SDL_image.h>

Graphics::Graphics(const Config& config) : config_(config) {
  int window_flags = SDL_WINDOW_RESIZABLE;
  int renderer_flags = 0;

  if (config_.fullscreen) window_flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
  if (config_.vsync) renderer_flags |= SDL_RENDERER_PRESENTVSYNC;
  if (config_.opengl) {
    window_flags |= SDL_WINDOW_OPENGL;
    renderer_flags |= SDL_RENDERER_ACCELERATED;
  }

  window_ = SDL_CreateWindow(
      config_.title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      config_.width, config_.height, window_flags);
  renderer_ = SDL_CreateRenderer(window_, -1, renderer_flags);

  if (!config_.fullscreen) set_window_size();

  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
  SDL_RenderSetLogicalSize(renderer_, config_.width, config_.height);
  SDL_RenderSetIntegerScale(renderer_, config_.intscale ? SDL_TRUE : SDL_FALSE);
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
    case FlipDirection::None:
      f = SDL_FLIP_NONE;
      break;

    case FlipDirection::Horizontal:
      f = SDL_FLIP_HORIZONTAL;
      break;

    case FlipDirection::Vertical:
      f = SDL_FLIP_VERTICAL;
      break;

    case FlipDirection::Both:
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

void Graphics::toggle_fullscreen() {
  config_.fullscreen = !config_.fullscreen;
  SDL_SetWindowFullscreen(window_, config_.fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);

  if (!config_.fullscreen) set_window_size();
}

void Graphics::draw_pixel(Point p, Color color) {
  set_color(color);
  SDL_RenderDrawPoint(renderer_, p.x, p.y);
}

void Graphics::draw_line(Point p1, Point p2, Color color) {
  set_color(color);
  if (p1.x == p2.x || p1.y == p2.y) {
    // SDL handles horizontal and vertical lines correctly
    SDL_RenderDrawLine(renderer_, p1.x, p1.y, p2.x, p2.y);
  } else {

    // Bresenham for angled lines
    const int dx = std::abs(p1.x - p2.x);
    const int dy = std::abs(p1.y - p2.y);

    if (dx < dy) {
      if (p1.y > p2.y) std::swap(p1, p2);

      for (int y = p1.y; y <= p2.y; ++y) {
        int x = (int)std::round((y - p1.y) / (float)(p2.y - p1.y) * (p2.x - p1.x) + p1.x);
        draw_pixel({x, y}, color);
      }

      /* TODO optimized implementation
      int e = 2 * dy - dx;
      int y = p1.y;

      while (p1.y <= p2.y) {
        while (e >= 0) {
          ++y;
          e -= 2 * dx;
        }
        SDL_RenderDrawLine(renderer_, p1.x, p1.y, p1.x, y - 1);

        p1.y = y;
        p1.x += p1.x < p2.x ? 1 : -1;
        e += 2 * dy;
      }
      */
    } else {
      if (p1.x > p2.x) std::swap(p1, p2);

      for (int x = p1.x; x <= p2.x; ++x) {
        int y = (int)std::round((x - p1.x) / (float)(p2.x - p1.x) * (p2.y - p1.y) + p1.y);
        draw_pixel({x, y}, color);
      }

      /* TODO optimized implementation
      int e = 2 * dx - dy;
      int x = p1.x;

      while (p1.x <= p2.x) {
        while (e >= 0) {
          ++x;
          e -= 2 * dy;
        }
        SDL_RenderDrawLine(renderer_, p1.x, p1.y, x - 1, p1.y);

        p1.x = x;
        p1.y += p1.y < p2.y ? 1 : -1;
        e += 2 * dx;
      }
      */
    }
  }
}

void Graphics::draw_rect(Point p1, Point p2, Color color, bool filled) {
  set_color(color);
  SDL_Rect rect = { p1.x, p1.y, p2.x - p1.x, p2.y - p1.y };
  filled ? SDL_RenderFillRect(renderer_, &rect) : SDL_RenderDrawRect(renderer_, &rect);
}

void Graphics::draw_circle(Point center, int r, Color color, bool filled) {
  set_color(color);
  int cx = r, cy = 0, error = 0;

  while (cx >= cy) {
    if (filled) {
      SDL_RenderDrawLine(renderer_, center.x - cx, center.y + cy, center.x + cx, center.y + cy);
      SDL_RenderDrawLine(renderer_, center.x - cy, center.y + cx, center.x + cy, center.y + cx);
      SDL_RenderDrawLine(renderer_, center.x - cx, center.y - cy, center.x + cx, center.y - cy);
      SDL_RenderDrawLine(renderer_, center.x - cy, center.y - cx, center.x + cy, center.y - cx);
    } else {
      SDL_Point points[8];

      points[0].x = points[7].x = center.x + cx;
      points[1].x = points[6].x = center.x + cy;
      points[2].x = points[5].x = center.x - cy;
      points[3].x = points[4].x = center.x - cx;

      points[0].y = points[3].y = center.y + cy;
      points[1].y = points[2].y = center.y + cx;
      points[4].y = points[7].y = center.y - cy;
      points[5].y = points[6].y = center.y - cx;

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

void Graphics::draw_triangle_top(Point p1, Point p2, Point p3, Color color) {
  assert(p2.y == p3.y);

  const float s1 = (float)(p2.x - p1.x) / (float)(p2.y - p1.y);
  const float s2 = (float)(p3.x - p1.x) / (float)(p3.y - p1.y);

  float x1 = p1.x, x2 = p1.x;
  for (int y = p1.y; y <= p2.y; ++y) {
    draw_line({(int)std::round(x1), y}, {(int)std::round(x2), y}, color);
    x1 += s1;
    x2 += s2;
  }
}

void Graphics::draw_triangle_bottom(Point p1, Point p2, Point p3, Color color) {
  assert(p1.y == p2.y);

  const float s1 = (p3.x - p2.x) / (float)(p3.y - p2.y);
  const float s2 = (p3.x - p1.x) / (float)(p3.y - p1.y);

  float x1 = p3.x, x2 = p3.x;
  for (int y = p3.y; y > p2.y; --y) {
    draw_line({(int)std::round(x1), y}, {(int)std::round(x2), y}, color);
    x1 -= s1;
    x2 -= s2;
  }
}

void Graphics::draw_triangle(Point p1, Point p2, Point p3, Color color, bool filled) {
  if (filled) {
    if (p1.y > p2.y) std::swap(p1, p2);
    if (p2.y > p3.y) std::swap(p2, p3);
    if (p1.y > p2.y) std::swap(p1, p2);

    if (p2.y == p3.y) {
      draw_triangle_top(p1, p2, p3, color);
    } else if (p1.y == p2.y) {
      draw_triangle_bottom(p1, p2, p3, color);
    } else {
      Point p4 = { p1.x + (p2.y - p1.y) * (p3.x - p1.x) / (p3.y - p1.y), p2.y };
      draw_triangle_top(p1, p2, p4, color);
      draw_triangle_bottom(p2, p4, p3, color);
    }

  } else {
    draw_line(p1, p2, color);
    draw_line(p2, p3, color);
    draw_line(p3, p1, color);
  }
}

SDL_Texture* Graphics::load_image(const std::string& file) {
  const std::string path("content/" + file);
  if (textures_.count(path) == 0) {
    SDL_Surface* surface = IMG_Load(path.c_str());

    if (surface == NULL) {
      fprintf(stderr, "Unable to load %s: %s\n", file.c_str(), IMG_GetError());
    }

    textures_[path] = SDL_CreateTextureFromSurface(renderer_, surface);
    SDL_FreeSurface(surface);
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

void Graphics::set_window_size() {
  SDL_SetWindowSize(window_,
      config_.width * config_.scale,
      config_.height * config_.scale);
}
