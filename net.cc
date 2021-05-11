#include "net.h"

#include <cassert>
#include <algorithm>

Packet::Packet(const std::string& s) {
  length = s.length();
  memcpy(data, s.c_str(), length);
}

Packet::Packet(const char* s) {
  length = strlen(s);
  memcpy(data, s, length);
}

bool Socket::open(const std::string& hostname, uint16_t port) {
  assert(socket_ == NULL);

  IPaddress ip;
  if (SDLNet_ResolveHost(&ip, hostname.c_str(), port) == -1) return false;
  socket_ = SDLNet_TCP_Open(&ip);

  return socket_ != NULL;
}

bool Socket::open(uint16_t port) {
  assert(socket_ == NULL);

  IPaddress ip;
  if (SDLNet_ResolveHost(&ip, NULL, port) == -1) return false;
  socket_ = SDLNet_TCP_Open(&ip);

  return socket_ != NULL;
}


void Socket::close() {
  assert(socket_ != NULL);

  SDLNet_TCP_Close(socket_);
  socket_ = NULL;
}

Socket Socket::accept() {
  assert(socket_ != NULL);
  return Socket(SDLNet_TCP_Accept(socket_));
}

void Socket::send(Packet p) {
  assert(socket_ != NULL);
  SDLNet_TCP_Send(socket_, p.data, p.length);
}

Packet Socket::receive() {
  assert(socket_ != NULL);
  Packet p;
  p.length = SDLNet_TCP_Recv(socket_, p.data, Packet::kMaxPacketSize);
  return p;
}

bool Socket::ready() const {
  assert(socket_ != NULL);
  return SDLNet_SocketReady(socket_) != 0;
}

SocketSet::SocketSet(size_t max) : max_size_(max), set_(SDLNet_AllocSocketSet(max)) {}

SocketSet::~SocketSet() {
  SDLNet_FreeSocketSet(set_);
}

bool SocketSet::add(const Socket& s) {
  // TODO check for errors
  int n = SDLNet_TCP_AddSocket(set_, s.socket_);
  if (n == -1) {
    return false;
  } else {
    sockets_.push_back(s);
    return true;
  }
}

void SocketSet::remove(const Socket& s) {
  sockets_.erase(std::remove(sockets_.begin(), sockets_.end(), s), sockets_.end());
  SDLNet_TCP_DelSocket(set_, s.socket_);
}

int SocketSet::ready_count(uint32_t timeout) const {
  return SDLNet_CheckSockets(set_, timeout);
}

Server::Server(uint16_t port) : set_(kMaxSockets) {
  SDLNet_Init();
  server_.open(port);
  set_.add(server_);
}

Server::~Server() {
  SDLNet_Quit();
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
      const Packet p = client.receive();

      if (p.length == 0) {
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
