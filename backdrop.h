#pragma once

#include <string>

#include "graphics.h"

class Backdrop {

  public:

    Backdrop(const std::string& file);

    void draw(Graphics& graphics) const;
    virtual void update() {}

  private:

    std::string file_;

};
