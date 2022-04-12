#include "graphics.h"

#include <cassert>
#include <cmath>
#include <utility>

#include <SDL2/SDL_image.h>

Graphics::Graphics(const Config& config) : config_(config) {
  int window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL;
  int renderer_flags = SDL_RENDERER_ACCELERATED;

  if (config_.fullscreen) window_flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
  if (config_.vsync) renderer_flags |= SDL_RENDERER_PRESENTVSYNC;

  window_ = SDL_CreateWindow(
      config_.title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      scaled_width(), scaled_height(), window_flags);
  renderer_ = SDL_CreateRenderer(window_, -1, renderer_flags);

  if (!config_.fullscreen) set_window_size();

  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
  SDL_RenderSetLogicalSize(renderer_, config_.width, config_.height);
  SDL_RenderSetIntegerScale(renderer_, config_.intscale ? SDL_TRUE : SDL_FALSE);
  SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND);
}

Graphics::~Graphics() {
  for (auto& i : textures_) {
    SDL_DestroyTexture(i.second);
  }

  SDL_DestroyRenderer(renderer_);
  SDL_DestroyWindow(window_);
}

void Graphics::blit(const std::string& file) {
  SDL_Texture* texture = load_image(file);
  SDL_RenderCopy(renderer_, texture, nullptr, nullptr);
}

void Graphics::blit(const std::string& file, const Rect source, const Rect dest) {
  SDL_Texture* texture = load_image(file);
  SDL_Rect s = { source.x, source.y, source.w, source.h };
  SDL_Rect d = { dest.x, dest.y, dest.w, dest.h };
  SDL_RenderCopy(renderer_, texture, &s, &d);
}

void Graphics::blit_flip(const std::string& file, const Rect source, const Rect dest, bool hflip, bool vflip) {
  int f = SDL_FLIP_NONE;
  if (hflip) f |= SDL_FLIP_HORIZONTAL;
  if (vflip) f |= SDL_FLIP_VERTICAL;

  SDL_Texture* texture = load_image(file);
  SDL_Rect s = { source.x, source.y, source.w, source.h };
  SDL_Rect d = { dest.x, dest.y, dest.w, dest.h };
  SDL_RenderCopyEx(renderer_, texture, &s, &d, 0, nullptr, (SDL_RendererFlip)f);
}

void Graphics::blit_rot(const std::string& file, const Rect source, const Rect dest, const float angle, const Point center) {
  SDL_Texture* texture = load_image(file);
  SDL_Rect s = { source.x, source.y, source.w, source.h };
  SDL_Rect d = { dest.x, dest.y, dest.w, dest.h };
  SDL_Point c = { center.x, center.y };
  SDL_RenderCopyEx(renderer_, texture, &s, &d, angle * 180.0f / M_PI, &c, SDL_FLIP_NONE);
}

void Graphics::flip() {
  SDL_RenderPresent(renderer_);
}

void Graphics::clear() {
  clear(0x000000ff);
}

void Graphics::clear(Color color) {
  set_color(color);
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
  draw_line_prim(p1, p2);
}

void Graphics::draw_line_prim(Point p1, Point p2) {
  const int dx = std::abs(p1.x - p2.x);
  const int dy = std::abs(p1.y - p2.y);

  if (dx > dy) {
    if (p1.x > p2.x) std::swap(p1, p2);
    const float a = (p2.y - p1.y) / (float)(p2.x - p1.x);
    for (int x = p1.x; x <= p2.x; ++x) {
      SDL_RenderDrawPoint(renderer_, x, (int)std::round((x - p1.x) * a + p1.y));
    }
  } else {
    if (p1.y > p2.y) std::swap(p1, p2);
    const float a = (p2.x - p1.x) / (float)(p2.y - p1.y);
    for (int y = p1.y; y <= p2.y; ++y) {
      SDL_RenderDrawPoint(renderer_, (int)std::round((y - p1.y) * a + p1.x), y);
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

void Graphics::draw_triangle_top(Point p1, Point p2, Point p3) {
  assert(p2.y == p3.y);

  const float s1 = (float)(p2.x - p1.x) / (float)(p2.y - p1.y);
  const float s2 = (float)(p3.x - p1.x) / (float)(p3.y - p1.y);

  float x1 = p1.x, x2 = p1.x;
  for (int y = p1.y; y <= p2.y; ++y) {
    draw_line_prim({(int)std::round(x1), y}, {(int)std::round(x2), y});
    x1 += s1;
    x2 += s2;
  }
}

void Graphics::draw_triangle_bottom(Point p1, Point p2, Point p3) {
  assert(p1.y == p2.y);

  const float s1 = (p3.x - p2.x) / (float)(p3.y - p2.y);
  const float s2 = (p3.x - p1.x) / (float)(p3.y - p1.y);

  float x1 = p3.x, x2 = p3.x;
  for (int y = p3.y; y > p2.y; --y) {
    draw_line_prim({(int)std::round(x1), y}, {(int)std::round(x2), y});
    x1 -= s1;
    x2 -= s2;
  }
}

void Graphics::draw_triangle(Point p1, Point p2, Point p3, Color color, bool filled) {
  set_color(color);

  if (filled) {
    if (p1.y > p2.y) std::swap(p1, p2);
    if (p2.y > p3.y) std::swap(p2, p3);
    if (p1.y > p2.y) std::swap(p1, p2);

    if (p2.y == p3.y) {
      draw_triangle_top(p1, p2, p3);
    } else if (p1.y == p2.y) {
      draw_triangle_bottom(p1, p2, p3);
    } else {
      Point p4 = { p1.x + (p2.y - p1.y) * (p3.x - p1.x) / (p3.y - p1.y), p2.y };
      draw_triangle_top(p1, p2, p4);
      draw_triangle_bottom(p2, p4, p3);
    }

  } else {
    draw_line_prim(p1, p2);
    draw_line_prim(p2, p3);
    draw_line_prim(p3, p1);
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

void Graphics::set_color(uint32_t color) {
  const int r = (color & 0xff000000) >> 24;
  const int g = (color & 0x00ff0000) >> 16;
  const int b = (color & 0x0000ff00) >> 8;
  const int a = (color & 0x000000ff);

  SDL_SetRenderDrawColor(renderer_, r, g, b, a);
}

void Graphics::set_window_size() {
  SDL_SetWindowSize(window_, scaled_width(), scaled_height());
  SDL_SetWindowMinimumSize(window_, width(), height());
}
