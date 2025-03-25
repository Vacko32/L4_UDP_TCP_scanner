#ifndef TCPscan_h
#define TCPscan_h

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/ip_icmp.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <pcap.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>
#include <array>
#include <chrono>
#include <cstring>
#include <future>
#include <iostream>
#include <optional>
#include <string>
#include <thread>
#include <vector>

struct pseudo_header {
  u_int32_t source_address;
  u_int32_t dest_address;
  u_int8_t placeholder;
  u_int8_t protocol;
  u_int16_t tcp_length;
};

struct ipv6_pseudo_header {
  struct in6_addr source_address;
  struct in6_addr dest_address;
  u_int8_t zeros[3];
  u_int32_t length;
  u_int8_t next_header;
};

class tcp_socket {
 public:
  tcp_socket(char* domain, int port, std::vector<std::string> interfaces_addresses,
             std::string mainInterface);
  ~tcp_socket() { freeaddrinfo(res); };
  int check_adresses(std::string addr1, std::string addr2);
 private:
  struct iphdr ip_header;
  struct ip6_hdr ip6_header;
  struct tcphdr tcp_header;
  struct pseudo_header psh;
  int sockfd;
  void setsocket(std::string ip_adress, int st, std::string bind_adr);
  int craft_packet(std::string source, std::string destination, int port);
  struct addrinfo hints;
  struct addrinfo* res;
  std::vector<std::string> adresses;         // adresses that needs to be scanned
  std::vector<std::string> source_adresses;  // for example 127.0.0.1 localhost
};

#endif