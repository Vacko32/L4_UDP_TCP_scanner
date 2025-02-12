// should include help

#include "main.h"

int mainn(int argc, char** argv) {
  Args x(argc, argv);
  for (auto i : x.Ports){
    cout << i << endl;
  }
  for (auto i : x.UPorts){
    cout << i << endl;
  }
  return 0;
}