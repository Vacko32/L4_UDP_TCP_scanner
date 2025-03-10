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

#include <algorithm>
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

struct pseudo_header_ipv4 {
  uint32_t src_addr;
  uint32_t dst_addr;
  uint8_t zero;
  uint8_t protocol;
  uint16_t tcp_len;
};

class Packet {
  // ipv4 header
  struct iphdr ip4;
  // ipv6 header
  struct ip6_hdr ip6;
  // tcp header
  struct tcphdr tcp;
  // udp header
  struct udphdr udp;
  int v4_or_v6;
  int tcp_or_udp;
  // pseudo tcp header
  pseudo_header_ipv4 p_tcp;
  // pseudo udp header
  pseudo_header_ipv4 p_udp;

 public:
  Packet(IpType version, int port, int sockfd, int destionation_addr, int protocol_type);
  [[nodiscard]] std::vector<char>
  serialize();  // compiler warns when value from serialize is not used, turns packet to bytes.
  void addIpHeader(IpType version, int protocol_type);
  void addProtocolHeader(...);
};

class Socket {
  struct addrinfo hints;
  struct addrinfo* res;
  int sockfd;
  IpType iptype;

 public:
  Socket(char* domain, int port, int socktype);
  void createAndSendPacket(int port, int sock_type, int sockfd, std::string destionation_addr,
                           std::string protocol_type);
  void setsocket(std::string ip_adress, int st);
  ~Socket() { freeaddrinfo(res); };
};

#endif