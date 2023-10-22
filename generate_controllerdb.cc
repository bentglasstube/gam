#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <db file> ..." << std::endl;
    return 1;
  }

  std::cout << R"(#include "controllerdb.h"

const unsigned char ControllerDB::data[] = {
)";

  for (int i = 1; i < argc; ++i) {
    std::cerr << "Adding controller mappings from " << argv[i] << std::endl;
    std::ifstream input(argv[i]);
    for (std::string line; std::getline(input, line);) {
      if (line.empty()) continue;
      if (line[0] == '#') continue;
      std::cout << "  \"" << line << "\"" << std::endl;
    }
  }

  std::cout << R"(};

void ControllerDB::AddMappings() {
  SDL_GameControllerAddMappingsFromRW(SDL_RWFromConstMem(data, sizeof(data)), 1);
}
)";

  return 0;
}
