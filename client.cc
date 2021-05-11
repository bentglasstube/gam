#include "client.h"

#include <cassert>

Client::Client(const std::string& hostname, uint16_t port) :
  set_(1), hostname_(hostname), port_(port), waiting_(false) {}

void Client::check() {
  if (!waiting_) return;

  if (set_.ready_count(0) > 0) {
    const std::string p = client_.receive();

    if (p.length() == 0) {
      error();
    } else {
      set_.remove(client_);
      client_.close();
      waiting_ = false;

      receive(p);
    }
  }
}

void Client::send(const std::string& data) {
  assert(!waiting_);

  client_.open(hostname_, port_);

  if (client_) {
    client_.send(data);
    set_.add(client_);
    waiting_ = true;
  } else {
    error();
  }
}
