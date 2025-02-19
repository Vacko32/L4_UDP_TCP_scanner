#include "sockets.hh"

Socket::Socket(char* domain, int port, int socktype) {
  memset(&hints, 0,
         sizeof hints);         // empying the struct, source:
                                // https://beej.us/guide/bgnet/html/#getaddrinfoprepare-to-launch
  hints.ai_family = AF_UNSPEC;  // v4 and v6
  hints.ai_socktype = socktype;
  hints.ai_flags = AI_PASSIVE;

  int status = getaddrinfo(domain, std::to_string(port).c_str(), &hints, &res);

  if (status != 0) {
    std::cerr << "getaddrinfo: " << gai_strerror(status) << std::endl;
    throw std::runtime_error("Error(3): getaddrinfo failed");
  }
  determineipv();
}

void Socket::setiphead() {
  if (hints.ai_socktype == IPPROTO_UDP) {}
}

void Socket::setsocket() {
  int x = 0;
  int st = 0;
  switch (hints.ai_socktype) {
    case (SOCK_STREAM):
      st = IPPROTO_TCP;
      break;
    case (SOCK_DGRAM):
      st = IPPROTO_UDP;
      break;
  }
  if (iptype == IpType::v4) {
    x = PF_INET;
  } else {
    x = PF_INET6;
  }

  sockfd = socket(x, SOCK_RAW, st);  // last arg 0, it will look for correct one
  if (sockfd == -1) {
    throw std::runtime_error("Error(4): Socket creation failed");
  }
};

void Socket::startscan() {
  // we have header setup, now we will iterate through the adrreses located in res, based on these
  // adrreses we will scan and change the header then we will send the packet

};

void Socket::determineipv() {
  if (res->ai_family == AF_INET) {
    iptype = IpType::v4;
  } else {
    iptype = IpType::v6;
  }
}

template <typename IpHeader, typename ProtocolHeader>
void Packet<IpHeader, ProtocolHeader>::addProtocolHeader(u_int16_t port, int protocoltype) {};