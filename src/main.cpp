// should include help

#include "main.h"

int main(int argc, char** argv) {
  Args x(argc, argv);
  for (auto i : x.Ports){
    cout << i << endl;
  }
  return 0;
}