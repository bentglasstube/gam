#include "game.h"
#include "screen.h"
#include "sprite.h"
#include "text.h"

class ExampleScreen : public Screen {
  public:

    ExampleScreen() :
      text_("text.png"),
      controller_("controller.png", 0, 0, 256, 96),
      indicator_("indicator.png", 0, 0, 16, 16) {}

    bool update(const Input& input, Audio& audio, unsigned int) override {
      buttons_[0] = input.key_held(Input::Button::Up);
      buttons_[1] = input.key_held(Input::Button::Left);
      buttons_[2] = input.key_held(Input::Button::Right);
      buttons_[3] = input.key_held(Input::Button::Down);
      buttons_[4] = input.key_held(Input::Button::Select);
      buttons_[5] = input.key_held(Input::Button::Start);
      buttons_[6] = input.key_held(Input::Button::B);
      buttons_[7] = input.key_held(Input::Button::A);

      if (input.key_pressed(Input::Button::Up))    audio.play_sample("sq0.wav");
      if (input.key_pressed(Input::Button::Left))  audio.play_sample("sq1.wav");
      if (input.key_pressed(Input::Button::Right)) audio.play_sample("sq2.wav");
      if (input.key_pressed(Input::Button::Down))  audio.play_sample("sq3.wav");

      if (input.key_pressed(Input::Button::Select)) audio.play_sample("misc0.wav");
      if (input.key_pressed(Input::Button::Start))  audio.play_sample("misc1.wav");

      if (input.key_pressed(Input::Button::B)) audio.play_sample("noise0.wav");
      if (input.key_pressed(Input::Button::A)) audio.play_sample("noise1.wav");

      return true;
    }

    void draw(Graphics& graphics) const override {
      const int dx = graphics.width() / 2 - 128;
      const int dy = graphics.height() / 2 - 48;

      text_.draw(graphics, "libgam example", graphics.width() / 2, dy - 24, Text::Alignment::Center);

      controller_.draw(graphics, dx, dy);

      if (buttons_[0]) indicator_.draw(graphics,  32 + dx, 34 + dy);
      if (buttons_[1]) indicator_.draw(graphics,  18 + dx, 48 + dy);
      if (buttons_[2]) indicator_.draw(graphics,  46 + dx, 48 + dy);
      if (buttons_[3]) indicator_.draw(graphics,  32 + dx, 62 + dy);
      if (buttons_[4]) indicator_.draw(graphics,  88 + dx, 60 + dy);
      if (buttons_[5]) indicator_.draw(graphics, 120 + dx, 60 + dy);
      if (buttons_[6]) indicator_.draw(graphics, 168 + dx, 60 + dy);
      if (buttons_[7]) indicator_.draw(graphics, 208 + dx, 60 + dy);
    }

    Screen* next_screen() const override {
      return nullptr;
    }

  private:

    Text text_;
    Sprite controller_, indicator_;
    bool buttons_[8];
};


int main(int, char**) {
  Game::Config config;

  config.graphics.title = "gam example";
  config.graphics.width = 256;
  config.graphics.height = 240;
  config.graphics.fullscreen = true;

  Game game(config);
  game.loop(new ExampleScreen());

  return 0;
}
