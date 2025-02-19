#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <string>
#include <vector>

#include "../src/helptext.cc"
#include "../src/parsearg.cc"
#include "../src/sockets.cc"
TEST_CASE("parsearg") {
  const char* input[] = {"./main", "--interface", "eth0", "-u", "53", "www.vutbr.cz"};
  int argc = 6;
  Args x(argc, const_cast<char**>(input));
  cout << x.mainInterface;
  REQUIRE(x.mainInterface == "eth0");
  for (auto i : x.UPorts) {
    cout << i;
  }
  REQUIRE(x.domain == "www.vutbr.cz");
}

TEST_CASE("parsearg2") {
  const char* input[] = {"./main", "--interface", "eth0", "-u", "53-56", "www.vutbr.cz"};
  int argc = 6;
  Args x(argc, const_cast<char**>(input));
  cout << x.mainInterface;
  REQUIRE(x.mainInterface == "eth0");
  for (auto i : x.UPorts) {
    cout << i;
  }
  REQUIRE(x.domain == "www.vutbr.cz");
}

TEST_CASE("parsearg3") {
  const char* input[] = {"./main", "--interface", "eth0", "-u", "53,56", "www.vutbr.cz"};
  int argc = 6;
  Args x(argc, const_cast<char**>(input));
  cout << x.mainInterface;
  REQUIRE(x.mainInterface == "eth0");
  for (auto i : x.UPorts) {
    cout << i;
  }
  REQUIRE(x.domain == "www.vutbr.cz");
}

TEST_CASE("parsearg4") {
  const char* input[]
      = {"./main", "--interface", "eth0", "-u", "53,56", "www.vutbr.cz", "-w", "3000"};
  int argc = 8;
  Args x(argc, const_cast<char**>(input));
  cout << x.mainInterface;
  REQUIRE(x.mainInterface == "eth0");
  for (auto i : x.UPorts) {
    cout << i;
  }
  REQUIRE(x.domain == "www.vutbr.cz");
  REQUIRE(x.W == 3000);
}

TEST_CASE("sockets1") {
  const char* input[] = {"./main", "--interface", "eth0", "-u", "53,56", "vut.cz"};
  int argc = 6;
  Args x(argc, const_cast<char**>(input));
  Socket s(const_cast<char*>(x.domain.c_str()), x.UPorts[0], 1);
}

TEST_CASE("sockets2") {
  const char* input[] = {"./main", "--interface", "eth0", "-t", "53,56", "vut.cz"};
  int argc = 6;
  Args x(argc, const_cast<char**>(input));
  Socket s(const_cast<char*>(x.domain.c_str()), x.UPorts[0], 1);
}
