#include <SDL2/SDL.h>

class ControllerDB {
 public:
  static void AddMappings();

 private:
  static const unsigned char data[];
};
