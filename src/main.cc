// should include help

#include "main.hh"

int main(int argc, char** argv) {
  Args x(argc, argv);
  for (auto i : x.Ports) {
    cout << i << endl;
  }
  for (auto i : x.UPorts) {
    cout << i << endl;
  }
  return 0;
}