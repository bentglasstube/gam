#pragma once

#include <string>

#include "audio.h"
#include "graphics.h"
#include "input.h"

class Screen {

  public:

    virtual void init() {}
    virtual bool update(Input& input, Audio& audio, unsigned int elapsed) = 0;
    virtual void draw(Graphics& graphics) const = 0;
    virtual Screen* next_screen() { return NULL; }
    virtual std::string get_music_track() const { return ""; }

    bool process_input(Input& input);
};
