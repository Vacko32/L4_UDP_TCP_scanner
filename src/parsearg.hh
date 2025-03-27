#ifndef parsearg_h
#define parsearg_h
#include <ifaddrs.h>
#include <netdb.h>

#include <algorithm>
#include <cctype>
#include <iostream>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <thread>
#include "TCPscan.hh"
#include "UDPscan.hh"
#include "helptext.hh"
using namespace std;

class Interface {
 public:
  string name;
  string ip;
  Interface(string n, string i) : name(n), ip(i) {}
};

class Args {
 public:
  Args(int l, char** dat);
  void scan_udp();
  void scan_tcp();
  void setupinterfaces();
  void fill_scan_destinations_udp(char* domain);

 private:
  std::mutex mtx;
  std::mutex mtx2_print;
  std::queue<int> port_queue;
  std::condition_variable cv;
  static constexpr int MAX_THREADS = 50;  // shared between all instances of the class
  void executor();
  void executor_tcp();
  void fill_scan_destinations_tcp(char* domain);
  bool nextisflag(vector<string> v, int idx);
  void parsePort(vector<string> ports);
  void printhelp();
  void portchceck(int c);
  int len;
  int W;
  bool list;             // indicates if interface is specified
  string mainInterface;  // eth0
  string domain;         // www.vutbr.cz
  vector<string> data;   // arguments all
  vector<int> Ports;
  vector<int> UPorts;
  vector<Interface> interfaces;  // all interfaces and ips
  vector<string> mainInterface_addr;
  vector<string> scan_destinations;
};

#endif