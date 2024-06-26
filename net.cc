#include "net.h"

#include <algorithm>
#include <cassert>
#include <sstream>

bool Socket::open(const std::string& hostname, uint16_t port) {
  IPaddress ip;
  if (SDLNet_ResolveHost(&ip, hostname.c_str(), port) == -1) return false;
  socket_ = SDLNet_TCP_Open(&ip);

  return socket_ != NULL;
}

bool Socket::open(uint16_t port) {
  close();

  IPaddress ip;
  if (SDLNet_ResolveHost(&ip, NULL, port) == -1) return false;
  socket_ = SDLNet_TCP_Open(&ip);

  return socket_ != NULL;
}


void Socket::close() {
  SDLNet_TCP_Close(socket_);
  socket_ = NULL;
}

Socket Socket::accept() {
  assert(socket_ != NULL);
  return Socket(SDLNet_TCP_Accept(socket_));
}

void Socket::send(const std::string& p) {
  assert(socket_ != NULL);
  SDLNet_TCP_Send(socket_, p.c_str(), p.length());
}

const std::string Socket::receive() {
  assert(socket_ != NULL);

  std::ostringstream buffer;
  char data[kChunkSize];

  while (true) {
    int length = SDLNet_TCP_Recv(socket_, data, kChunkSize);
    if (length > 0) buffer << std::string(data, length);
    if (length < kChunkSize) return buffer.str();
  }
}

bool Socket::ready() const {
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
