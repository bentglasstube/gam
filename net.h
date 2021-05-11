#pragma once

#include <SDL2/SDL_net.h>

#include <string>
#include <vector>

class Socket {
  public:

    Socket() : socket_(NULL) {}
    Socket(const std::string& host, uint16_t port) { open(host, port); }
    Socket(uint16_t port) { open(port); }

    bool open(const std::string& host, uint16_t port);
    bool open(uint16_t port);

    void close();

    Socket accept();
    void send(const std::string& p);
    const std::string receive();

    operator bool() const { return socket_ != NULL; }
    bool ready() const;

  private:

    static constexpr int kChunkSize = 1024;

    TCPsocket socket_;

    Socket(TCPsocket socket) : socket_(socket) {}

    struct Hash {
      inline size_t operator()(const Socket& s) const {
        return (size_t)s.socket_;
      }
    };

    friend class SocketSet;
    friend bool operator==(const Socket& l, const Socket& r);
};

inline bool operator==(const Socket& l, const Socket& r) {
  return l.socket_ == r.socket_;
}

class SocketSet {
  public:

    SocketSet(size_t max);
    ~SocketSet();

    typedef std::vector<Socket>::iterator iterator;
    typedef std::vector<Socket>::const_iterator const_iterator;

    iterator begin() { return sockets_.begin(); }
    const_iterator begin() const { return sockets_.cbegin(); }
    const_iterator cbegin() const { return sockets_.cbegin(); }

    iterator end() { return sockets_.end(); }
    const_iterator end() const { return sockets_.cend(); }
    const_iterator cend() const { return sockets_.cend(); }

    bool empty() const { return sockets_.empty(); }
    bool size() const { return sockets_.size(); }
    bool max_size() const { return max_size_; }

    bool add(const Socket& s);
    void remove(const Socket& s);

    int ready_count(uint32_t timeout=0) const;

  private:

    size_t max_size_;
    SDLNet_SocketSet set_;
    std::vector<Socket> sockets_;

};
