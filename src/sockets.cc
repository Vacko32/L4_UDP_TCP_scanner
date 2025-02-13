#include "sockets.hh"

using namespace std;

Socket::Socket(char* domain, int port) {
  memset(&hints, 0, sizeof hints);  // empying the struct
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  status = getaddrinfo(domain, to_string(port).c_str(), &hints, &res);
  if (status != 0) {
    cerr << "getaddrinfo: " << gai_strerror(status) << endl;
    exit(1);
  }
}

void Socket::printinfo() {
  struct addrinfo* p;
  for (p = res; p != NULL; p = p->ai_next) {
    // showing that in p->ai_addr we have the address that can be translated
    // cast type SHOULD ONLY BE CALLED TO IPV4
    struct sockaddr_in* ipv4 = (struct sockaddr_in*)p->ai_addr;
    char ip[16];
    inet_ntop(p->ai_family, &(ipv4->sin_addr), ip, sizeof ip);
    cout << "IPv4: " << ip << endl;
  }
}
