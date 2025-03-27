#include "parsearg.hh"

int GLOBAL_TIMEOUT = 5000;
std::string Interfaceee;
bool Args::nextisflag(vector<string> v, int idx) {
  if (v[idx] == "-t" || v[idx] == "--pt" || v[idx] == "-u" || v[idx] == "--pu" || v[idx] == "-w"
      || v[idx] == "--wait") {
    return true;
  }
  return false;
}

void Args::portchceck(int c) {
  if (c > 65535) {
    throw std::runtime_error("Error(1): Bad arguments, read help! ./main");
  }
}

void Args::scan_udp() {

  
  // first we find destinations for udp 
  fill_scan_destinations_udp(const_cast<char*>(domain.c_str()));
  
  for (int uport : UPorts) {
    port_queue.push(uport);
  }

  
  
  std::vector<std::thread> threads;
  for (int i = 0; i < MAX_THREADS; i++) {
    threads.emplace_back(std::thread(&Args::executor, this));
  }
  for (auto& t : threads) {
    t.join();
  }
  
}

void Args::executor(){
  while(true){
    int port;
    {
      std::unique_lock<std::mutex> lock(mtx);
      if (port_queue.empty()){
        return;
      }
      port = port_queue.front();
      port_queue.pop();
    }
    udp_socket s(const_cast<char*>(domain.c_str()), port, SOCK_DGRAM, mainInterface_addr, mainInterface, scan_destinations);
    std::lock_guard<std::mutex> lock(mtx2_print);
    s.print_output();
  }
}

void Args::scan_tcp() {
  for (int port : Ports) {
    tcp_socket s(const_cast<char*>(domain.c_str()), port, mainInterface_addr, mainInterface);
  }
}

void Args::printhelp() {
  std::cout << HELP_TEXT;
  exit(0);
}


void Args::fill_scan_destinations_tcp(char* domain){
  // first we null scan_destination
  while(!scan_destinations.empty()){
    scan_destinations.pop_back();
  }
  struct addrinfo hints;
  struct addrinfo* res;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  char ipstr[INET6_ADDRSTRLEN];
  int status = getaddrinfo(domain, NULL, &hints, &res);
  if (status != 0) {
    std::cerr << "getaddrinfo: " << gai_strerror(status) << std::endl;
    std::cerr << "domain: " << domain << std::endl;
    throw std::runtime_error("Error(3): getaddrinfo failed");
  }
  struct addrinfo *p;
  for (p = res; p != NULL; p = p->ai_next){
    void *addr;
    const char *ipver;
    if (p->ai_family == AF_INET)
    { // IPv4
      struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
      addr = &(ipv4->sin_addr);
      
    }
    else if (p->ai_family == AF_INET6) {
      struct sockaddr_in6 *ipv6 = reinterpret_cast<struct sockaddr_in6 *>(p->ai_addr);
      addr = &(ipv6->sin6_addr);
    }
    if(addr){
    if (inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr)) != nullptr) {
        std::cout << "IP TCP ADDED: " << ipstr << std::endl;
        scan_destinations.push_back(ipstr);
      }
    }
  }
}



void Args::fill_scan_destinations_udp(char* domain){
  struct addrinfo hints;
  struct addrinfo* res;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE;
  char ipstr[INET6_ADDRSTRLEN];
  int status = getaddrinfo(domain, NULL, &hints, &res);
  if (status != 0) {
    std::cerr << "getaddrinfo: " << gai_strerror(status) << std::endl;
    std::cerr << "domain: " << domain << std::endl;
    throw std::runtime_error("Error(3): getaddrinfo failed");
  }
  struct addrinfo *p;
  for (p = res; p != NULL; p = p->ai_next){
    void *addr;
    const char *ipver;
    if (p->ai_family == AF_INET)
    { // IPv4
      struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
      addr = &(ipv4->sin_addr);
      
    }
    else if (p->ai_family == AF_INET6) {
      struct sockaddr_in6 *ipv6 = reinterpret_cast<struct sockaddr_in6 *>(p->ai_addr);
      addr = &(ipv6->sin6_addr);
    }
    if(addr){
    if (inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr)) != nullptr) {
        std::cout << "IP ADDED: " << ipstr << std::endl;
        scan_destinations.push_back(ipstr);
      }
    }
  }

  return;
}

Args::Args(int l, char** dat) {
  int udpflag = 0;
  bool hasI;
  bool hasP;
  bool hasW;
  bool validD;
  len = l;
  hasI = false;
  vector<string> v;
  if (l == 1) {
    printhelp();
  }

  for (int i = 1; i < l; i++) {
    v.push_back(dat[i]);
  }
  data = v;
  size_t j = 0;
  while (j < data.size()) {  // while reading the arguments
    // we want to indentify which flag are we processing

    if ((data[j] == "-i" || data[j] == "--interface") && hasI == false) {
      hasI = true;
      // we can be at the end so care for segfault
      if (j != data.size() - 1) {
        j++;
        bool x = nextisflag(v, j);
        if (!x) {
          mainInterface = data[j];
          Interfaceee = mainInterface;
          list = false;
          j++;
        } else {
          list = true;
        }
      } else {
        list = true;
      }

    } else if (data[j] == "-t" || data[j] == "--pt" || data[j] == "-u" || data[j] == "--pu") {
      udpflag = 0;
      if (data[j] == "-u" || data[j] == "--pu") {
        udpflag = 1;
      }
      // we will be given prost and they can be range, num, or numbers
      // we accepts number chars, -, and ,
      // other chars exits
      j++;  // now we are at the porst arguent
      hasP = true;
      if (j == data.size()) {
        throw std::runtime_error("Error(2): Bad arguments, read help! ./main");
      }
      size_t readidx = 0;
      while (isdigit(data[j][readidx])) {
        readidx++;
      }
      // we reached the end of the FIRST POTENTIONAL number
      // now we ask are we at the end or no
      if (readidx == data[j].size()) {  // SHOULDNT BE MINUS ONE
        // we are at the endstring substring = data[j].substr(0, readidx);
        string substring2 = data[j].substr(0, readidx);
        int numx = stoi(substring2);
        if (udpflag == 1) {
          portchceck(numx);
          UPorts.push_back(numx);
        } else {
          portchceck(numx);
          Ports.push_back(numx);
        }
        j++;
      }

      else {
        string substring = data[j].substr(0, readidx);
        int num = stoi(substring);  // the first port we read
        // we have scanned the first number
        // we look ahead to see if the next char is - or , to determine if range or list is given
        if (data[j][readidx] == '-') {
          int idx2 = readidx + 1;
          while (isdigit(data[j][idx2])) {
            idx2++;
          }
          int num2 = stoi(data[j].substr(readidx + 1, idx2));
          // we have the second number and we want to push ports into the vector
          for (int i = num; i <= num2; i++) {
            if (udpflag == 1) {
              portchceck(i);
              UPorts.push_back(i);
            } else {
              portchceck(i);
              Ports.push_back(i);
            }
          }
        } else if (data[j][readidx] == ',') {
          // we will be reading a list of ports, because of that we will
          // scan always until end of data[j][idx] or we reach the comma
          if (udpflag == 1) {
            portchceck(num);
            UPorts.push_back(num);
          } else {
            portchceck(num);
            Ports.push_back(num);
          }
          readidx += 1;
          int temp = readidx;
          while (readidx < data[j].size()) {
            while (isdigit(data[j][readidx])) {
              readidx++;
            }
            string sub = data[j].substr(temp, readidx);
            int num = stoi(sub);
            if (udpflag == 1) {
              portchceck(num);
              UPorts.push_back(num);
            } else {
              portchceck(num);
              Ports.push_back(num);
            }
            temp = readidx + 1;
            readidx++;
          }
        } else {
          // error
          printhelp();
          throw std::runtime_error("Error(3): Bad arguments, read help! ./main");
        }
        j++;
      }
    } else if ((data[j] == "-w" || data[j] == "--wait")) {
      hasW = true;
      j++;
      if (j >= data.size()) {
        printhelp();
        throw std::runtime_error("Error(4): Bad arguments, read help! ./main");
      }
      // now we error check
      try {
        int timeout = stoi(data[j]);
        if (timeout <= 0) {
          printhelp();
          throw std::runtime_error("Error(5): Bad arguments negative timeout, read help! ./main");
        }
        W = timeout;
        GLOBAL_TIMEOUT = W;
      } catch (const std::invalid_argument& e) {
        printhelp();
        throw std::runtime_error("Error(6): Bad arguments, read help! ./main");
      }
      j++;
    } else if (validD != true) {  // set the val name or ip address
      validD = true;
      domain = data[j];
      j++;
    } else {
      // return error
      printhelp();
      throw std::runtime_error("Error(7): Bad arguments, read help! ./main currentnly on data[j]:"
                               + data[j]);
    }
  }
  // checks if input has necessarry requeirements
  if (validD != true) {
    printhelp();
    throw std::runtime_error("Error(8): Bad arguments, read help! ./main");
  }
  if (hasP == false && list != true) {
    printhelp();
    throw std::runtime_error("Error(9): Bad arguments, read help! ./main");
  }
}

// https://man7.org/linux/man-pages/man3/getifaddrs.3.html SOURCE CITATION NEEDED
void Args::setupinterfaces() {
  struct ifaddrs *ifaddr, *ifa;
  int family, s;
  char host[NI_MAXHOST];

  if (getifaddrs(&ifaddr) == -1) {
    throw std::runtime_error("Error(3): getifaddrs failed");
  }

  // we got a linked list of possible interfaces

  for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
    if (ifa->ifa_addr == NULL) {
      continue;
    }

    family = ifa->ifa_addr->sa_family;

    if (family == AF_PACKET) {
      continue;  // skip if it is a packet
    }

    s = getnameinfo(ifa->ifa_addr,
                    (family == AF_INET) ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6),
                    host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
    if (s != 0) {
      throw std::runtime_error("Error(3): getnameinfo failed");
    }
    Interface temp = Interface(ifa->ifa_name, host);
    
    interfaces.push_back(temp);
  }

  if (list == false) {
    // we want to check if given interface is valid
    bool found = false;
    for (auto i : interfaces) {
      if (i.name == mainInterface) {
        found = true;
        mainInterface_addr.push_back(i.ip);
      }
    }

    if (found == false) {
      printhelp();
      throw std::runtime_error("Error(11): Bad arguments INVALID INTERFACE, read help! ./main");
    }
  }
  if (list == true) {
    std::cout << "Available Network Interfaces:\n";
    std::cout << "\n";
    for (auto i : interfaces) {
      std::cout << i.name << " " << i.ip << "\n";
    }
  } else {
    interfaces.erase(std::remove_if(interfaces.begin(), interfaces.end(),
                                    [&](auto& i) { return i.name != mainInterface; }),
                     interfaces.end());
  }
  freeifaddrs(ifaddr);
}
