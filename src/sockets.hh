#ifndef sockets_h
#define sockets_h
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/ip.h>   // For struct iphdr (IP header)
#include <netinet/tcp.h>  // For struct tcphdr (TCP header)
#include <netinet/udp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <string>
// to create a socket, we need a port, and a valid domain or ip adrress
// we will create sockets, then push them into some queue or something, and scan from there

enum ipType {
  v4,
  v6,
};

class Socket {
 public:
  // values
  struct addrinfo hints;
  struct addrinfo* res;
  int status;
  int sockfd;
  bool isTcp;
  ipType iptype;

  // creationg based on deepseek suggestion
  char packet[1024];  // should be enough because we do not send any messages
  struct iphdr* iph;  // both of these structure will be pointing at the packet
  struct tcphdr* tcph;
  struct udphdr* udph;

  // functions
  Socket(char* domain, int port, int tcp);
  void printinfo();
  void sendpacket();
  void setsocket();
  void findiptype();
  void startscan();
  void determineipv();
  void setiphead(int ipt);
  ~Socket() { freeaddrinfo(res); };
};

#endif