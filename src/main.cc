// should include help

#include "main.hh"

using namespace std;
// bool isPortOpen(const string& ip, int port) {}

int main(int argc, char** argv) {
  try {
    Args x(argc, argv);
    Socket s(const_cast<char*>(x.domain.c_str()), x.UPorts[0]);
    s.printinfo();
    return 0;
  } catch (exception& e) {
    cout << e.what() << endl;
    return 1;
  }

  // args are parsed, and set in attributes
  // now we want to look what we will be scanning.
  // then we will do the scan

  /**
   * struct addrinfo {
    int              ai_flags;     // AI_PASSIVE, AI_CANONNAME, etc.
    int              ai_family;    // AF_INET, AF_INET6, AF_UNSPEC
    int              ai_socktype;  // SOCK_STREAM, SOCK_DGRAM
    int              ai_protocol;  // use 0 for "any"
    size_t           ai_addrlen;   // size of ai_addr in bytes
    struct sockaddr *ai_addr;      // struct sockaddr_in or _in6
    char            *ai_canonname; // full canonical hostname

    struct addrinfo *ai_next;      // linked list, next node
    };
   */

  // we will load the struct and then call getaddrinfo()

  /**
   *  struct sockaddr {
    unsigned short    sa_family;    // address family, AF_xxx
    char              sa_data[14];  // 14 bytes of protocol address
    };
   */
  // sockaddr HOLDS SOCKET ADDRESS INFORMATION OF MANY TYPES SOCKET

  // IMPORTANT: And this is the important bit: a pointer to a struct sockaddr_in can be cast to a
  // pointer to a struct sockaddr and vice-versa

  /*
      struct sockaddr_in {
    short int          sin_family;  // Address family, AF_INET
    unsigned short int sin_port;    // Port number USE HTONS !!
    struct in_addr     sin_addr;    // Internet address
    unsigned char      sin_zero[8]; // Same size as struct sockaddr
};

can be cast to struct sockaddr!!
  */

  // we will start by scanning tcp
  // assume we have just one port and one domain given, it is and tcp port
}