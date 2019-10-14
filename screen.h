#pragma once

#include <string>

#include "audio.h"
#include "graphics.h"
#include "input.h"

class Screen {

  public:

    Screen();
    virtual ~Screen();

    virtual void init() {}
    virtual bool update(const Input& input, Audio& audio, unsigned int elapsed) = 0;
    virtual void draw(Graphics& graphics) const = 0;
    virtual Screen* next_screen() const { return nullptr; }
    virtual std::string get_music_track() const { return ""; }

    float fps() const;
    void count_frame(unsigned int elapsed);

  private:

    static constexpr int kFpsFrames = 30;
    unsigned int ticks_[kFpsFrames], counter_;
};
