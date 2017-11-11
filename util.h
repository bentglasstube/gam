#pragma once

#include <cstddef>

class Util {
  public:
    static unsigned int random_seed();

  private:
    static size_t sysrandom(void* dst, size_t length);
};
