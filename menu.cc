#include "menu.h"

Menu::Menu(const std::string& file) : text_(file), selected_(0) {}

void Menu::next() {
  if (selected_ < choices_.size() - 1) ++selected_;
}

void Menu::prev() {
  if (selected_ > 0) --selected_;
}

void Menu::select(unsigned int n) {
  if (n < choices_.size()) selected_ = n;
}

void Menu::add_entry(const std::string& item) {
  choices_.push_back(item);
}

void Menu::draw(Graphics& graphics, int x, int y) const {
  for (unsigned int i = 0; i < choices_.size(); ++i) {
    if (selected_ == i) text_.draw(graphics, ">", x, y + 16 * i);
    text_.draw(graphics, choices_[i], x + 16, y + 16 * i);
  }
}

unsigned int Menu::selected() const {
  return selected_;
}
