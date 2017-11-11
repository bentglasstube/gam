#pragma once

#include <cstddef>

class Util {
  public:
    static unsigned int random_seed();

    template <class T> class CastHash {
      public:
        inline size_t operator()(T const& t) const {
          return static_cast<size_t>(t);
        }
    };

  private:
    static size_t sysrandom(void* dst, size_t length);
};
