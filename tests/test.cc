#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <string>
#include <vector>

#include "../src/parsearg.cc"

TEST_CASE("parsearg") {
  const char* input[] = {"./main", "--interface", "eth0", "-u", "53", "www.vutbr.cz"};
  int argc = 6;
  Args x(argc, const_cast<char**>(input));
  cout << x.Interface;
  REQUIRE(x.Interface == "eth0");
  for (auto i : x.UPorts) {
    cout << i;
  }
  REQUIRE(x.domain == "www.vutbr.cz");
}

TEST_CASE("parsearg2") {
  const char* input[] = {"./main", "--interface", "eth0", "-u", "53-56", "www.vutbr.cz"};
  int argc = 6;
  Args x(argc, const_cast<char**>(input));
  cout << x.Interface;
  REQUIRE(x.Interface == "eth0");
  for (auto i : x.UPorts) {
    cout << i;
  }
  REQUIRE(x.domain == "www.vutbr.cz");
}

TEST_CASE("parsearg3") {
  const char* input[] = {"./main", "--interface", "eth0", "-u", "53,56", "www.vutbr.cz"};
  int argc = 6;
  Args x(argc, const_cast<char**>(input));
  cout << x.Interface;
  REQUIRE(x.Interface == "eth0");
  for (auto i : x.UPorts) {
    cout << i;
  }
  REQUIRE(x.domain == "www.vutbr.cz");
}