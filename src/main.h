#ifndef main_h
#define main_h
#include <iostream>
#include <string>
#include <vector>
using namespace std;
class Args {
 public:
  int len;
  vector<string> data;
  Args(int l, char** dat);

  // functions for parsing
  // checking regexes of inputs

  // what combinations can be done what is mandatory what is not

  // -i OR --interface is interface, list of activa interface
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