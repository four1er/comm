#pragma once

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <variant>

#include "poller.h"

namespace NNet {
class TInitializer {
public:
  TInitializer();
};

class TAddress {
public:
  TAddress() = default;
  TAddress(const std::string &addr, int port);
  TAddress(sockaddr_in addr);
  TAddress(sockaddr_in6 addr);
  TAddress(sockaddr *addr, socklen_t len);

  const std::variant<sockaddr_in, sockaddr_in6> &Addr() const;
  std::pair<const sockaddr *, int> RawAddr() const;
  bool operator==(const TAddress &other) const;
  int Domain() const;
  TAddress WithPort(int port) const;
  std::string ToString() const;

private:
  std::variant<sockaddr_in, sockaddr_in6> Addr_ = {};
};

class TSocketOps {
public:
  TSocketOps(TPollerBase &poller, int domain, int type);
  TSocketOps(int fd, TPollerBase &poller);
  TSocketOps() = default;

  TPollerBase *Poller() { return Poller_; }

protected:
  int Create(int domain, int type);
  int Setup(int s);

  TPollerBase *Poller_ = nullptr;
  int Fd_ = -1;
};

} // namespace NNet
