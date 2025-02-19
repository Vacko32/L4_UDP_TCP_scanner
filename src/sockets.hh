// mv
#ifndef sockets_h
#define sockets_h
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <array>
#include <cstring>
#include <iostream>
#include <optional>
#include <string>
#include <vector>
// to create a socket, we need a port, and a valid domain or ip adrress
// we will create sockets, then push them into some queue or something, and scan from there

enum class IpType {
  v4,
  v6,
};

template <typename IpHeader, typename ProtocolHeader>
class Packet {
  IpHeader* ipheader;
  ProtocolHeader* protocolheader;

 public:
  Packet() = default;
  [[nodiscard]] std::vector<char>
  serialize();  // compiler warns when value from serialize is not used, turns packet to bytes.
  void addIpHeader(IpType version);
  void addProtocolHeader(...);
};

class Socket {
  struct addrinfo hints;
  struct addrinfo* res;
  int sockfd;
  IpType iptype;

 public:
  Socket(char* domain, int port, int socktype);
  void createAndSendPacket();
  void setsocket();
  void startscan();
  void determineipv();
  void setiphead();
  ~Socket() { freeaddrinfo(res); };
};

#endif