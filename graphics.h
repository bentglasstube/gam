#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_opengl_glext.h>

#include <string>
#include <unordered_map>
#include <vector>

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

class Graphics {
  public:

    struct Config {
      Config() : fullscreen(true), vsync(true), intscale(true), scale(1) {}
      bool fullscreen, vsync, intscale;
      int scale;
      std::string title;
      int width, height;
    };

    struct Point { int x, y; };
    struct Rect { int x, y, w, h; };

    typedef uint32_t Color;

    Graphics(const Config& config);
    ~Graphics();

    void blit(const std::string& file);
    void blit(const std::string& file, const Rect source, const Rect dest);
    void blit_flip(const std::string& file, const Rect source, const Rect dest, bool hflip, bool vflip);
    void blit_rot(const std::string& file, const Rect source, const Rect dest, const float angle, const Point center);
    void flip();
    void clear();
    void clear(Color color);
    void toggle_fullscreen();

    void draw_pixel(Point p, Color color);
    void draw_line(Point p1, Point p2, Color color);
    void draw_rect(Point p1, Point p2, Color color, bool filled);
    void draw_circle(Point center, int radius, Color color, bool filled);
    void draw_triangle(Point p1, Point p2, Point p3, Color color, bool filled);

    int width() const { return config_.width; }
    int height() const { return config_.height; }
    int scaled_width() const { return config_.width * config_.scale; }
    int scaled_height() const { return config_.height * config_.scale; }

    SDL_Texture* load_image(const std::string& file);

  private:

    Config config_;
    std::unordered_map<std::string, SDL_Texture*> textures_;
    SDL_Window* window_;
    SDL_Renderer* renderer_;

    void set_color(uint32_t color);
    void set_window_size();
    void draw_triangle_top(Point p1, Point p2, Point p3);
    void draw_triangle_bottom(Point p1, Point p2, Point p3);
    void draw_line_prim(Point p1, Point p2);
};
