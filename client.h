#pragma once

#include "net.h"

class Client {
  public:

    Client(const std::string& hostname, uint16_t port);
    virtual ~Client() { client_.close(); }

    virtual void receive(const std::string&) = 0;
    virtual void error() {}

    void check();
    bool connected() { return client_; }
    bool waiting() { return waiting_; }
    void send(const std::string& data);

  private:

    Socket client_;
    SocketSet set_;
    std::string hostname_;
    uint16_t port_;
    bool waiting_;

};
