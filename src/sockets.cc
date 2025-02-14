#include "sockets.hh"

Socket::Socket(char* domain, int port, int tcp) {
  using namespace std;
  if (tcp == 1) {
    isTcp = true;
  } else {
    isTcp = false;
  }
  cout << "CREATING SOCKET" << domain;

  memset(&hints, 0, sizeof hints);  // empying the struct
  hints.ai_family = AF_UNSPEC;      // important for both ipv6 and ipv4
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  status = getaddrinfo(domain, to_string(port).c_str(), &hints, &res);
  if (status != 0) {
    cerr << "getaddrinfo: " << gai_strerror(status) << endl;
    throw std::runtime_error("Error(3): getaddrinfo failed");
  }
  cout << "GOT ADDR" << "\n";
  cout << "PORT: " << port << "\n";
  cout << "DOMAIN: " << domain << "\n";
  cout << "HINTS" << hints.ai_family << "\n";
  cout << "hints" << ((struct sockaddr_in*)res->ai_addr)->sin_port;
}

void Socket::printinfo() {
  using namespace std;

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

void Socket::findiptype() {
  // based on the value in res, we determine the ip type

};

// before casting this function we should somehow determine if we are sending tcp or udp and if we
// have ipv4 or ipv6

void Socket::setiphead(int ipt) {
  using namespace std;

  iph = (struct iphdr*)packet;                             // iphdr is the first thing in the packet
  tcph = (struct tcphdr*)(packet + sizeof(struct iphdr));  // tcphdr is after iphdr
  udph = (struct udphdr*)(packet + sizeof(struct iphdr));  // udphdr is after iphdr

  // filling IP header
  iph->ihl = 5;
  iph->version = 4;
  iph->tos = 0;
  iph->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr);
  iph->id = htons(54321);  // id of this packet
  iph->frag_off = 0;
  iph->ttl = 255;
  iph->protocol = IPPROTO_TCP;
  iph->check = 0;  // set to 0 before calculating checksum
  // based on the iptype we can detrmine where the source and destrionation address will be
  iph->saddr = ((struct sockaddr_in*)res->ai_addr)->sin_addr.s_addr;  // source address
  iph->daddr = ((struct sockaddr_in*)res->ai_addr)->sin_addr.s_addr;  // destination address

  // depending on tcp or udp, after ip will be the tcp or udp header
  // iph->check = checksum((unsigned short*)iph, sizeof(struct iphdr));
  // now we will find out if we are sending tcp or udp
  if (isTcp) {                                                   // SYN PACKET MOST IMPORTANT ONE
    tcph->dest = ((struct sockaddr_in*)res->ai_addr)->sin_port;  // destination port
    tcph->source = htons(11691);                                 // random source port
    tcph->seq = 0;
    tcph->ack_seq = 0;
    tcph->doff = 5;  // tcp header size
    tcph->syn = 1;
    tcph->fin = 0;
    tcph->ack = 0;
    tcph->urg = 0;
    tcph->psh = 0;
    tcph->rst = 0;
    tcph->window = htons(5840);  // maximum allowed window size, uses big endian
    tcph->check = 0;             // leave checksum 0 for now
    tcph->urg_ptr = 0;
    // now we will calculate the checksum
    // need to be done
  } else {
    udph->check = 0;
    udph->dest = ((struct sockaddr_in*)res->ai_addr)->sin_port;
    udph->len = 0;
    udph->source = htons(11692);
  }
}

void Socket::setsocket() {
  using namespace std;

  sockfd = socket(PF_INET, SOCK_RAW, getprotobyname("tcp")->p_proto);
  if (sockfd == -1) {
    throw std::runtime_error("Error(4): Socket creation failed");
  }
};

void Socket::startscan() {
  // we have header setup, now we will iterate through the adrreses located in res, based on these
  // adrreses we will scan and change the header then we will send the packet
  using namespace std;
};

void Socket::determineipv() {
  using namespace std;
  if (res->ai_family == AF_INET) {
    iptype = v4;
  } else {
    iptype = v6;
  }
}