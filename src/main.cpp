// should include help

#include "main.h"

Args::Args(int l, char** dat) {
  len = l;
  vector<string> v;

  // checking desired length (l) of input by user
  // we have gotten char**, want to translate it into <vector> of strings

  for (int i = 0; i < l; i++) {
    v.push_back(dat[i]);
  }
  data = v;
}

int main(int argc, char** argv) {
  Args x(argc, argv);
  cout << x.len;
  return 0;
}