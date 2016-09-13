#pragma once

#include <string>

#include "audio.h"
#include "graphics.h"
#include "input.h"

class Screen {

  public:

    Screen();

    virtual void init() {}
    virtual bool update(const Input& input, Audio& audio, unsigned int elapsed) = 0;
    virtual void draw(Graphics& graphics) const = 0;
    virtual Screen* next_screen() { return NULL; }
    virtual std::string get_music_track() const { return ""; }

    bool process_input(Input& input);

    float fps() const;
    void count_frame(unsigned int elapsed);

  private:

    static const int FPS_FRAMES = 30;
    unsigned int ticks_[FPS_FRAMES], counter_;
};
