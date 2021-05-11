#include "server.h"

#include <cstdio>

Server::Server(uint16_t port) : set_(kMaxSockets) {
  SDL_Init(SDL_INIT_TIMER | SDL_INIT_EVENTS);
  SDLNet_Init();

  server_.open(port);
  set_.add(server_);
}

Server::~Server() {
  for (Socket& s : set_) {
    set_.remove(s);
    s.close();
  }

  SDLNet_Quit();
  SDL_Quit();
}

void Server::loop() {
  SDL_Event event;
  unsigned int last_update = SDL_GetTicks();
  bool running = true;

  while (running) {
    const unsigned int next_update = SDL_GetTicks();
    const unsigned int frame_ticks = next_update - last_update;

    update(frame_ticks);
    poll();

    last_update = next_update;

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) running = false;
    }
  }

  shutdown();
}

void Server::poll(uint32_t timeout) {
  int ready = set_.ready_count(timeout);

  if (ready == 0) return;

  if (server_.ready()) {
    Socket client = server_.accept();
    if (client) {
      set_.add(client);
      connect(client);
    }

    --ready;
  }

  for (Socket& client : set_) {
    if (client == server_) continue;

    if (client.ready()) {
      const std::string p = client.receive();

      if (p.length() == 0) {
        disconnect(client);
        set_.remove(client);
      } else {
        receive(client, p);
      }

      --ready;
    }

    if (ready == 0) break;
  }
}
