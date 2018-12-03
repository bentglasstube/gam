#include "util.h"

#include <iostream>

#ifdef _WIN32

#include <windows.h>
#include <wincrypt.h>

bool acquire_context(HCRYPTPROV *ctx) {
  if (!CryptAcquireContext(ctx, nullptr, nullptr, PROV_RSA_FULL, 0)) {
    return CryptAcquireContext(ctx, nullptr, nullptr, PROV_RSA_FULL, CRYPT_NEWKEYSET);
  }
  return true;
}

size_t Util::sysrandom(void* dst, size_t length) {
  HCRYPTPROV ctx;
  if (!acquire_context(&ctx)) {
    std::cerr << "Unable to initialize windows crypto library\n";
    return 0;
  }

  BYTE* buffer = static_cast<BYTE*>(dst);

  if (!CryptGenRandom(ctx, length, buffer)) {
    std::cerr << "Unable to generate random bytes\n";
    return 0;
  }

  if (!CryptReleaseContext(ctx, 0)) {
    std::cerr << "Unable to release windows crypto library\n";
    return 0;
  }

  return length;
}

#else

#include <fstream>

size_t Util::sysrandom(void* dst, size_t length) {
  char* buffer = static_cast<char*>(dst);
  std::ifstream stream("/dev/urandom", std::ios_base::binary | std::ios_base::in);
  stream.read(buffer, length);
  return length;
}

#endif

unsigned int Util::random_seed() {
  unsigned int seed;
  sysrandom(&seed, sizeof(seed));
  return seed;
}
