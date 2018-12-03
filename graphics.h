#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <map>

class Graphics {
  public:

    struct Config {
      Config() : fullscreen(true), opengl(true), vsync(true), intscale(true), scale(1) {}
      bool fullscreen, opengl, vsync, intscale;
      int scale;
      std::string title;
      int width, height;
    };

    enum class FlipDirection { None, Horizontal, Vertical, Both };

    Graphics(const Config& config);
    ~Graphics();

    void blit(const std::string& file, const SDL_Rect* srect, const SDL_Rect* drect);
    void blit_ex(const std::string& file, const SDL_Rect* s, const SDL_Rect* d, const float angle, const SDL_Point* center, const FlipDirection flip);
    void flip();
    void clear();
    void toggle_fullscreen();

    void draw_pixel(int x, int y, int color);
    void draw_line(int x1, int y1, int x2, int y2, int color);
    void draw_rect(const SDL_Rect* rect, int color, bool filled);
    void draw_circle(int x, int y, int r, int color, bool filled);

    int width() const { return config_.width; }
    int height() const { return config_.height; }

    SDL_Texture* load_image(const std::string& file);

  private:

    typedef std::map<std::string, SDL_Texture*> TextureMap;

    Config config_;
    TextureMap textures_;
    SDL_Window* window_;
    SDL_Renderer* renderer_;

    void set_color(int color);
    void set_window_size();
};
