#include "helptext.hh"

#include <string>
const std::string HELP_TEXT = R"(
Usage: ./ipk-l4-scan [options] <domain-name | ip-address>
Available options:
  -i <interface>        Specify interface or list of active interfaces
                         (If no value specified, the list of interfaces will be printed)
  -t <ports> OR --pt <ports>                  Set TCP port ranges (e.g., --pt 22 for SSH port)
  -u <ports> OR --pu <ports>             Set UDP port ranges (e.g., --pu 1-65535 for all UDP ports)
  -w <timeout>          Set timeout in milliseconds (default: 5000 ms)
                         Timeout is for a single port scan
  --wait <timeout>      Timeout in milliseconds for single port scan
  <domain-name | ip>    Specify domain name or IP address to scan

Example usage:
./ipk-l4-scan -i eth0 --pt 21,22,143 --pu 53,67 localhost

This will scan the following ports on localhost (127.0.0.1):
PORT       STATE
21/tcp     closed
22/tcp     open
143/tcp    filtered
53/udp     closed
67/udp     open
)";
