#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <string>
#include <vector>

#include "../src/parsearg.cc"

TEST_CASE("parsearg") {
  const char* input[] = {"./main", "--interface", "eth0", "-u", "53,67", "www.vutbr.cz"};
  int argc = 6;
  Args x(argc, const_cast<char**>(input));
  CHECK(x.Interface == "eth0");
  REQUIRE(x.domain == "www.vutbr.cz");
  std::vector<int> expectedPorts = {53, 67};
  for (size_t i = 0; i < expectedPorts.size(); ++i) {
    CHECK(x.UPorts[i] == expectedPorts[i]);
  }
}
