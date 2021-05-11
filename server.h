#pragma once

#include "net.h"

class Server {
  public:

    Server(uint16_t port);
    virtual ~Server();

    void loop();
    void poll(uint32_t timeout=0);

    virtual void connect(Socket&) {}
    virtual void disconnect(Socket&) {}
    virtual void receive(Socket&, const Packet&) = 0;
    virtual void update(unsigned int elapsed) = 0;
    virtual void shutdown() {}

  private:

    static constexpr size_t kMaxSockets = 128;

    Socket server_;
    SocketSet set_;
};
