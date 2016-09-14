#pragma once

#include <string>
#include <vector>

#include "graphics.h"
#include "text.h"

class Menu {
  public:

    Menu(const std::string& file);

    void next();
    void prev();
    void select(unsigned int n);

    void add_entry(const std::string& item);

    void draw(Graphics& graphics, int x, int y) const;
    unsigned int selected() const;

  private:

    Text text_;
    unsigned int selected_;
    std::vector<std::string> choices_;
};
