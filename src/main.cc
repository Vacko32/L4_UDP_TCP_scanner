// should include help

#include "main.hh"

using namespace std;

int main(int argc, char** argv) {
  try {
    Args x(argc, argv);
    x.setupinterfaces();
    x.scan_udp();
    return 0;
  } catch (exception& e) {
    cout << e.what() << endl;
    return 1;
  }
}