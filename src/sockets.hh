#ifndef sockets_h
#define sockets_h
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <string>
// to create a socket, we need a port, and a valid domain or ip adrress
// we will create sockets, then push them into some queue or something, and scan from there

class Socket {
 public:
  struct addrinfo hints;
  struct addrinfo* res;
  int status;
  int sockfd;
  Socket(char* domain, int port);
  ~Socket() { freeaddrinfo(res); };
  void printinfo();
};

#endif