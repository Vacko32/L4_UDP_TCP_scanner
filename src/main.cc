// should include help

#include "main.hh"

using namespace std;

int main(int argc, char** argv) {
  try {
    Args x(argc, argv);
    x.setupinterfaces();

    for (int port : x.Ports) {
      Socket s(const_cast<char*>(x.domain.c_str()), port, SOCK_STREAM);
    }

    for (int uport : x.UPorts) {
      Socket s(const_cast<char*>(x.domain.c_str()), uport, SOCK_DGRAM);
    }

    return 0;
  } catch (exception& e) {
    cout << e.what() << endl;
    return 1;
  }
}