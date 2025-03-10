#include "sockets.hh"

Socket::Socket(char *domain, int port, int socktype) {
  memset(&hints, 0,
         sizeof hints);         // empying the struct, source:
                                // https://beej.us/guide/bgnet/html/#getaddrinfoprepare-to-launch
  hints.ai_family = AF_UNSPEC;  // v4 and v6
  hints.ai_socktype = socktype;
  hints.ai_flags = AI_PASSIVE;
  char ipstr[INET6_ADDRSTRLEN];
  int status = getaddrinfo(domain, std::to_string(port).c_str(), &hints, &res);

  if (status != 0) {
    std::cerr << "getaddrinfo: " << gai_strerror(status) << std::endl;
    throw std::runtime_error("Error(3): getaddrinfo failed");
  }
  // we got a linked list of possible adresses
  // we will iterate through them and if they are not in our adress vector we will add them
  struct addrinfo *p;
  std::vector<std::string> adresses;
  for (p = res; p != NULL; p = p->ai_next) {
    void *addr;
    const char *ipver;
    if (p->ai_family == AF_INET) {  // IPv4
      struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
      addr = &(ipv4->sin_addr);
      ipver = "IPv4";
    } else {  // IPv6
      struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
      addr = &(ipv6->sin6_addr);
      ipver = "IPv6";
    }

    inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
    printf("  %s: %s\n", ipver, ipstr);
    adresses.push_back(ipstr);
  }

  // for each address and for each adress of interfaces make packet and send it, port is determined
  // in the function
  for (std::string i : adresses) {
    /*for (std::string j : interfaces) {  // somehow pass
     setsocket(i, socktype);
   } */
  }
}

void Socket::setsocket(std::string ip_adress, int st) {
  int version;
  switch (hints.ai_socktype) {
    case (SOCK_STREAM):
      st = IPPROTO_TCP;
      break;
    case (SOCK_DGRAM):
      st = IPPROTO_UDP;
      break;
  }

  for (char i : ip_adress) {
    if (i == '.') {
      version = PF_INET;
      break;
    } else if (i == ':') {
      version = PF_INET6;
      break;
    }
  }

  sockfd = socket(version, SOCK_RAW, st);  // last arg 0, it will look for correct one
  if (sockfd == -1) {
    throw std::runtime_error("Error(4): Socket creation failed");
  }
};

Packet::Packet(IpType version, int port, int sockfd, int destionation_addr, int protocol_type) {
  // first we need to add an ip header
  addIpHeader(version, protocol_type);
  // then we need to add a protocol header
}

void Packet::addIpHeader(IpType version, int protocol_type) {
  if (version == IpType::v4) {
    ip4.ihl = 5;
    ip4.version = 4;
    ip4.tos = 0;
    ip4.tot_len = 0;
    ip4.id = 0;
    ip4.frag_off = 0;
    ip4.ttl = 255;
    ip4.protocol = protocol_type;
    ip4.check = 0;
    ip4.saddr = 0;
    ip4.daddr = 0;
  } else {
    ip6.ip6_flow = 0;
    ip6.ip6_vfc = 6;
    ip6.ip6_plen = 0;
    ip6.ip6_nxt = IPPROTO_TCP;
    ip6.ip6_hops = 255;
    ip6.ip6_src = in6addr_any;
    ip6.ip6_dst = in6addr_any;
  }
}