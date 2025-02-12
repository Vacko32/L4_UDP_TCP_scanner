// should include help

#include "main.h"

bool nextisflag(vector<string> v, int idx) {
  if (v[idx] == "-t" || v[idx] == "--pt" || v[idx] == "-u" || v[idx] == "--pu" || v[idx] == "-w"
      || v[idx] == "--wait") {
    return true;
  }
  return false;
}

Args::Args(int l, char** dat) {
  len = l;
  vector<string> v;
  for (int i = 0; i < l; i++) {
    v.push_back(dat[i]);
  }
  data = v;
  int j = 0;
  while (j < data.size()) {  // mby minus 1

    // we want to indentify which flag are we processing

    if (data[j] == "-i" || data[j] == "--interface") {
      hasI = true;
      // we can be at the end so care for segfault
      if (j != data.size() - 1) {
        bool x = nextisflag(v, j + 1);
        if (!x) {
          Interface = data[j + 1];
          list = false;
        } else {
          list = true;
        }
      } else {
        list = true;
      }

    } else if (data[j] == "-t" || data[j] == "--pt" || data[j] == "-u" || data[j] == "--pu") {
        } else if (data[j] == "-w" || data[j] == "--wait") {
    } else if (validD != true) {  // set the val name or ip address

    } else {
      // return error
      cerr << "Error: Bad arguments, read help!" << endl;
      exit(1);
    }
  }
}

int main(int argc, char** argv) {
  Args x(argc, argv);
  cout << x.len;
  return 0;
}