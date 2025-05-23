// mv
#ifndef UDPscan_h
#define UDPscan_h
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/ip_icmp.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <net/if.h>
#include <pcap.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/icmp6.h>  
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

// to create a socket, we need a port, and a valid domain or ip adrress
// we will create sockets, then push them into some queue or something, and scan from there

enum class IpType {
  v4,
  v6,
};


class output_udp{
  int port; 
  std::string ip;
  int open_closed;
  public:
  output_udp(int p, std::string i, int o) : port(p), ip(i), open_closed(o) {}
  void print_state();
};
class udp_socket {
  struct addrinfo hints;
  struct addrinfo* res;
  int ported_temp_comp;
  int sockfd;
  int compatible_ip;
  u_int16_t port_bind_num;
  IpType iptype;
  std::vector<output_udp> out;
  int open_closed;
  std::vector<std::string> adresses;  // adresses that needs to be scanned
  std::vector<std::string> source_adresses;
  int handle_udp_socket(const std::string& source, const std::string& destination,
                        const std::string& mainInterface, int port);
  int check_adresses(std::string addr1, std::string addr2);
  int sniff_session_activate(pcap_t* handle, int iptype);
  void createAndSendPacket(int port, int sock_type, int sockfd, std::string destionation_addr,
                           std::string protocol_type);
  void setsocket(std::string ip_adress, int st, std::string bind_adr, int port);

 public:
  udp_socket(char* domain, int port, int socktype, std::vector<std::string> interfaces_addresses,
             std::string mainInterace, std::vector<std::string>);

  ~udp_socket() { freeaddrinfo(res); };
  void print_output();
};



#endif