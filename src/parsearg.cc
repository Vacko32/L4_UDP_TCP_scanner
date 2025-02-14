#include "parsearg.hh"

bool Args::nextisflag(vector<string> v, int idx) {
  if (v[idx] == "-t" || v[idx] == "--pt" || v[idx] == "-u" || v[idx] == "--pu" || v[idx] == "-w"
      || v[idx] == "--wait") {
    return true;
  }
  return false;
}

void Args::portchceck(int c) {
  if (c > 65535) {
    throw std::runtime_error("Error(3): Bad arguments, read help! ./main");
  }
}

void Args::printhelp() {
  cout << "Usage: ./ipk-l4-scan [options] <domain-name | ip-address>" << endl;
  cout << "Available options:" << endl;
  cout << "  -i <interface>        Specify interface or list of active interfaces" << endl;
  cout << "                         (If no value specified, the list of interfaces will be printed)"
       << endl;
  cout << "  -t <ports> OR --pt <ports>                  Set TCP port ranges (e.g., --pt 22 for "
          "SSH port)"
       << endl;
  cout << "  -u <ports> OR --pu <ports>             Set UDP port ranges (e.g., --pu 1-65535 for "
          "all UDP ports)"
       << endl;
  cout << "  -w <timeout>          Set timeout in milliseconds (default: 5000 ms)" << endl;
  cout << "                         Timeout is for a single port scan" << endl;
  cout << "  --wait <timeout>      Timeout in milliseconds for single port scan" << endl;
  cout << "  <domain-name | ip>    Specify domain name or IP address to scan" << endl;

  cout << "\nExample usage:\n";
  cout << "./ipk-l4-scan -i eth0 --pt 21,22,143 --pu 53,67 localhost" << endl;
  cout << "\nThis will scan the following ports on localhost (127.0.0.1):\n";
  cout << "PORT       STATE\n";
  cout << "21/tcp     closed\n";
  cout << "22/tcp     open\n";
  cout << "143/tcp    filtered\n";
  cout << "53/udp     closed\n";
  cout << "67/udp     open\n";
}

Args::Args(int l, char** dat) {
  int udpflag = 0;
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
          Interface = data[j];
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
        throw std::runtime_error("Error(3): Bad arguments, read help! ./main");
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
          throw std::runtime_error("Error(3): Bad arguments, read help! ./main");
        }
        j++;
      }
    } else if ((data[j] == "-w" || data[j] == "--wait") && hasW == false) {
      hasW = true;
      j++;
      if (j >= data.size()) {
        throw std::runtime_error("Error(3): Bad arguments, read help! ./main");
      }
      // now we error check
      try {
        int timeout = stoi(data[j]);
        if (timeout <= 0) {
          throw std::runtime_error("Error(3): Bad arguments negative timeout, read help! ./main");
        }
        W = timeout;
      } catch (const std::invalid_argument& e) {
        throw std::runtime_error("Error(3): Bad arguments, read help! ./main");
      }
      j++;
    } else if (validD != true) {  // set the val name or ip address
      validD = true;
      domain = data[j];
      j++;
    } else {
      // return error
      throw std::runtime_error("Error(3): Bad arguments, read help! ./main");
    }
  }
  // checks if input has necessarry requeirements
  if (validD != true) {
    throw std::runtime_error("Error(3): Bad arguments, read help! ./main");
  }
  if (hasP == false) {
    throw std::runtime_error("Error(3): Bad arguments, read help! ./main");
  }
}

// https://man7.org/linux/man-pages/man3/getifaddrs.3.html
void Args::setupinterfaces() {
  struct ifaddrs *ifaddr, *ifa;
  int family, s;
  char host[NI_MAXHOST];

  if (getifaddrs(&ifaddr) == -1) {
    throw std::runtime_error("Error(3): getifaddrs failed");
  }

  std::cout << "Available Network Interfaces:\n";
}