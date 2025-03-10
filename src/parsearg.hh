#ifndef parsearg_h
#define parsearg_h
#include <ifaddrs.h>
#include <netdb.h>

#include <algorithm>
#include <cctype>
#include <iostream>
#include <vector>

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
  bool nextisflag(vector<string> v, int idx);
  void parsePort(vector<string> ports);
  void printhelp();
  void portchceck(int c);
  void setupinterfaces();
  int len;
  int W;
  bool list;             // indicates if interface is specified
  string mainInterface;  // eth0
  string domain;         // www.vutbr.cz
  vector<string> data;   // arguments all
  vector<int> Ports;
  vector<int> UPorts;
  vector<Interface> interfaces;  // all interfaces and ips
  string mainInterface_addr;
};

#endif