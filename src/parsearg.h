#ifndef parsearg_h
#define parsearg_h
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
using namespace std;
class Args {
 public:
  int len;
  vector<string> data;
  bool hasI;
  string Interface;
  bool hasP;
  vector<int> Ports;
  bool hasW;
  string W;
  bool validD;
  string domain;
  bool list;
  Args(int l, char** dat);
  bool nextisflag(vector<string> v, int idx);
  void parsePort(vector<string> ports);
  void printhelp();
  // functions for parsing
  // checking regexes of inputs

  // what combinations can be done what is mandatory what is not

  // -i OR --interface is interface or list of active interfaces
  // is being printed if value is not specified
  // additional info beyond inter list is welcomed

  // -t OR --pt, -u OR --pu port-ranges,
  // --pt 22 SSH PORT,
  // --pu 1-65535
  // --pt 22,23,24

  // -w 3000 OR --wait 3000 is timeout in MILISECONDS
  // the timeout is set to be for a single port scan
  // default is 5000

  // domain-name OR ip-address

  // ALL ARGUMENTS CAN BE IN ANY GIVEN ORDER

  /**
./ipk-l4-scan -i eth0 --pt 21,22,143 --pu 53,67 localhost

Interesting ports on localhost (127.0.0.1):
PORT STATE
21/tcp closed
22/tcp open
143/tcp filtered
53/udp closed
67/udp open
   */
};

#endif