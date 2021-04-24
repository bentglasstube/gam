#pragma once

#include "graphics.h"

class Rect {
  public:
    Rect();
    Rect(double left, double top, double right, double bottom);

    double top, left, right, bottom;

    bool intersect(const Rect& other) const;
    bool contains(double x, double y) const;

    double width() const { return right - left; }
    double height() const { return bottom - top; }

    void draw(Graphics& graphics, int xo, int yo, Graphics::Color color, bool filled) const;
};
