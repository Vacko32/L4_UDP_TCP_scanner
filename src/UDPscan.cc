#include "UDPscan.hh"

extern int GLOBAL_TIMEOUT;
extern std::string Interfaceee;
int timeout_split = -1;
/**
 * @brief Construct a new udp_socket:: udp_socket object, it will also complete a whole scan
 *
 * @param domain,   the domain on which we will be scanning
 * @param port,     port that needs to be scanned
 * @param socktype  type of the socket, SOCK_DGRAM only supported
 *
 * @param interfaces_addresses adrresses of interfaces that we will be scanning from, already loaded
 * @param mainInterface  name of the mainInterface etc. lo, eth0
 *
 *
 *
 */
udp_socket::udp_socket(char *domain, int port, int socktype,
                       std::vector<std::string> interfaces_addresses, std::string mainInterface, std::vector<std::string> dest_adresses)
{
  ported_temp_comp = port;
  for (auto &i : interfaces_addresses)
  {
    size_t percent_pos = i.find('%');
    if (percent_pos != std::string::npos)
    {
      i = i.substr(0, percent_pos);
    }
  }

  source_adresses = interfaces_addresses;

  adresses = dest_adresses;

  int checker = 0;
  // for each address and for each adress of interfaces make packet and send it, port is determined
  // in the function
  // if atleast on one address we get close, the port is considered as being closed.

  for (std::string i : adresses)
  {
    timeout_split = interfaces_addresses.size();
    checker = 0;

    for (std::string j : interfaces_addresses)
    { // somehow pass
      if (check_adresses(i, j) == 3)
      {
        continue;
      }
      setsocket(i, socktype, j, port); // hopefully correct
      int resp = handle_udp_socket(j, i, mainInterface, port);
      if (resp == 3)
      {
        output_udp temp = output_udp(port, i, 1); // closed add
        out.push_back(temp);

        checker = 1;
      }
      close(sockfd);

    }
    if (checker != 1)
    {

      output_udp temp = output_udp(port, i, 0); // open add
      out.push_back(temp);
    }
  }
  return;
}

/**
 * @brief Create a socket
 *
 *
 * @param ip_adress  adress on which we will be scanning
 * @param sock_type  type of the socket, SOCK_DGRAM only supported
 * @param bind_adr   adress on which we will be binding the socket
 */
void udp_socket::setsocket(std::string ip_adress, int sock_type, std::string bind_adr, int port)
{
  int version;

  for (char i : ip_adress)
  {
    if (i == '.')
    {
      version = PF_INET;
      break;
    }
    else if (i == ':')
    {
      version = PF_INET6;
      break;
    }
  }

  sockfd = socket(version, SOCK_DGRAM, IPPROTO_UDP);

  if (sockfd == -1)
  {
    throw std::runtime_error("Error(4): udp_socket creation failed");
  }

  if (version == PF_INET)
  {
    struct sockaddr_in src_addr;

    memset(&src_addr, 0, sizeof(src_addr));
    src_addr.sin_family = AF_INET;
    src_addr.sin_port = 0;
    src_addr.sin_addr.s_addr = inet_addr(bind_adr.c_str());
    if (bind(sockfd, (struct sockaddr *)&src_addr, sizeof(src_addr)) < 0)
    {
      std::cerr << "Failed to bind socket: " << strerror(errno) << std::endl;
      std::cerr << "bind address:" << bind_adr << std::endl;
      std::cerr << "ip address:" << ip_adress << std::endl;
      close(sockfd);
      throw std::runtime_error("Error(4): udp_socket creation failed");
    }

    // retrieve the given port
    struct sockaddr_in actual;
    socklen_t len = sizeof(actual);
    if (getsockname(sockfd, (struct sockaddr *)&actual, &len) == -1)
    {
      std::cerr << "Failed to get port: " << strerror(errno) << std::endl;
      close(sockfd);
      throw std::runtime_error("Error(4): udp_socket creation failed");
    }
    port_bind_num = actual.sin_port;
  }
  else
  {
    // we have ipv6
    struct sockaddr_in6 src_addr;
    memset(&src_addr, 0, sizeof(src_addr));
    src_addr.sin6_family = AF_INET6;
    src_addr.sin6_port = 0;

    src_addr.sin6_scope_id = if_nametoindex(Interfaceee.c_str());

    if (inet_pton(AF_INET6, bind_adr.c_str(), &src_addr.sin6_addr) != 1)
    {
      std::cerr << "Invalid source address: " << bind_adr << std::endl;
      close(sockfd);
      throw std::runtime_error("Error(4): udp_socket creation failed");
    }

    // now we can bind the socket
    if (bind(sockfd, (struct sockaddr *)&src_addr, sizeof(src_addr)) != 0)
    {
      std::cerr << "Failed to bind socket: " << strerror(errno) << std::endl;

      close(sockfd);
      throw std::runtime_error("Error(4): udp_socket creation failed");
    }
    // retrieve the given port
    struct sockaddr_in actual;
    socklen_t len = sizeof(actual);
    if (getsockname(sockfd, (struct sockaddr *)&actual, &len) == -1)
    {
      std::cerr << "Failed to get port: " << strerror(errno) << std::endl;
      close(sockfd);
      throw std::runtime_error("Error(4): udp_socket creation failed");
    }
    port_bind_num = actual.sin_port;
  }
};

/**
 * @brief Handle the UDP socket, send the packet and sniff for ICMP response
 *
 * @param source,       source adress
 * @param destination,  destination adress
 * @param mainInterface, main interface
 * @param port,         port that needs to be scanned
 * @return int,         3 if success, else other value
 */

int udp_socket::handle_udp_socket(const std::string &source, const std::string &destination,
                                  const std::string &mainInterface, int port)
{
  // binding the socket

  int x = check_adresses(source, destination);
  // Both addresses are IPv4 - proceed
  // Set device to sniff on
  char *device = const_cast<char *>(mainInterface.c_str());

  // Create sniffing session
  pcap_t *handle;
  char errbuf[PCAP_ERRBUF_SIZE];
  handle = pcap_create(device, errbuf);
  if (handle == nullptr)
  {
    std::cerr << "Couldn't open device " << device << ": " << errbuf << std::endl;
    return 0;
  }

  if (pcap_set_timeout(handle, 75) != 0)
  {
    std::cerr << "Couldn't set timeout: " << pcap_geterr(handle) << std::endl;
    pcap_close(handle);
    return 0;
  }

  if (pcap_activate(handle) != 0)
  {
    std::cerr << "Couldn't activate handle: " << pcap_geterr(handle) << std::endl;
    pcap_close(handle);

    return 0;
  }
  if (pcap_setnonblock(handle, 1, errbuf) == -1)
  {
    std::cerr << "Couldn't set non-blocking mode: " << errbuf << std::endl;
    pcap_close(handle);
    return 0;
  }
  char message[8] = "message";
  if (x == 1)
  {

    struct sockaddr_in dest;
    dest.sin_family = AF_INET;
    dest.sin_port = htons(port);
    if (inet_pton(AF_INET, destination.c_str(), &dest.sin_addr) <= 0)
    {
      std::cerr << "Invalid destination address: " << destination << std::endl;
      pcap_close(handle);
      return 0;
    }

    std::promise<int> found_icpm;
    std::future<int> future_result = found_icpm.get_future();
    // create a sniffing session before sending packet
    std::thread t1([this, handle, &found_icpm]()
                   {
      int ret = this->sniff_session_activate(handle, 0);
      found_icpm.set_value(ret); });

    // Send the UDP packet, after sniffer is created
    // small timeout
    std::chrono::milliseconds timeout(50);

    ssize_t bytes_sent = sendto(sockfd, message, sizeof(message), 0,
                                reinterpret_cast<struct sockaddr *>(&dest), sizeof(dest));
    if (bytes_sent == -1)
    {
      std::cerr << "Failed to send packet: " << strerror(errno) << std::endl;
      std::cerr << "destination address:" << destination << std::endl;
      std::cerr << "source address:" << source << std::endl;
      pcap_close(handle);
      t1.join();
      return 0;
    }

    int retValue = future_result.get();
    t1.join(); // join thred
    pcap_close(handle);
    return retValue;

    // Poll for ICMP responses
    // source https://www.tcpdump.org/pcap.html
  }

  else if (x == 2)
  {

    // implementation for IPV6

    struct sockaddr_in6 dest;
    dest.sin6_family = AF_INET6;
    dest.sin6_port = htons(port);
    if (inet_pton(AF_INET6, destination.c_str(), &dest.sin6_addr) <= 0)
    {
      std::cerr << "Invalid destination address: " << destination << std::endl;
      pcap_close(handle);
      return 0;
    }

    std::promise<int> found_icpm;
    std::future<int> future_result = found_icpm.get_future();
    // create a sniffing session before sending packet
    std::thread t1([this, handle, &found_icpm]()
                   {
      int ret = this->sniff_session_activate(handle, 1);
      found_icpm.set_value(ret); });

    // Send the UDP packet, after sniffer is created
    // small timeout
    std::chrono::milliseconds timeout(50);

    ssize_t bytes_sent = sendto(sockfd, message, sizeof(message), 0,
                                reinterpret_cast<struct sockaddr *>(&dest), sizeof(dest));
    if (bytes_sent == -1)
    {
      std::cerr << "Failed to send packet: " << strerror(errno) << std::endl;
      std::cerr << "destination address:" << destination << std::endl;
      std::cerr << "source address:" << source << std::endl;
      pcap_close(handle);
      t1.join();
      return 0;
    }

    int retValue = future_result.get();
    t1.join(); // join thred
    pcap_close(handle);
    return retValue;

    
  }
  else if (x == 3)
  {
    // IPV6 both implementation
    pcap_close(handle);
    return 1;
  }
}

/**
 * @brief sniff the session and activate it, looks for ICMP response
 *
 * @param handle, pcap_t handle
 * @return int, 3 if success, else other value
 */
int udp_socket::sniff_session_activate(pcap_t *handle, int iptype)
{
  // activate the handle, used in sniffing session

  // pcap documentation seeting filter
  std::string filter = "icmp or icmp6";

  struct bpf_program fp;
  if (pcap_compile(handle, &fp, filter.c_str(), 0, PCAP_NETMASK_UNKNOWN) == -1)
  {
    std::cerr << "Couldn't parse filter " << filter << ": " << pcap_geterr(handle) << std::flush;
    
    
    return 0;
  }
  if (pcap_setfilter(handle, &fp) == -1)
  {
    std::cerr << "Couldn't install filter " << filter << ": " << pcap_geterr(handle) << std::flush;
    pcap_freecode(&fp);
    
    return 0;
  }

  // after activation
  struct pcap_pkthdr *header;
  const u_char *packet;
  std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
  int res = 0;
  while (std::chrono::steady_clock::now() - start < std::chrono::milliseconds(GLOBAL_TIMEOUT))
  { // scaning for 3 seconds
    res = pcap_next_ex(handle, &header, &packet);

    if (res == 1 && iptype == 0)
    {
      const u_char *data_ptr = packet + 64;
      uint16_t value = *reinterpret_cast<const uint16_t *>(data_ptr);

      if (ntohs(value) == ported_temp_comp)
      {
        // Found the UDP response, so exit immediately
        pcap_freecode(&fp);
        
        return 3;
      }
      else
      {
        continue;
      }
    }
    else if (res == 1 && iptype == 1)
    { // A packet was captured
      const u_char *data_ptr = packet + 104;
      uint16_t value = *reinterpret_cast<const uint16_t *>(data_ptr);
      
      if (ntohs(value) == ported_temp_comp)
      {
        // Found the UDP response, so exit immediately
        pcap_freecode(&fp);
        
        return 3;
      }
      else
      {
        continue;
      }
    }
    else if (res == -1)
    {
      pcap_freecode(&fp);
      
      return 0;
    }
  }

  pcap_freecode(&fp);
  
  return 1;
}
/**
 * @brief check if the adresses are of the same type
 *
 * @param addr1, adress 1
 * @param addr2, adress 2
 * @return int, 1 if both are IPv4, 2 if both are IPv6, 3 if they are not of the same type
 */
int udp_socket::check_adresses(std::string addr1, std::string addr2)
{
  int type1 = 0, type2 = 0;
  for (char i : addr1)
  {
    if (i == '.')
    {
      type1 = PF_INET;
      break;
    }
    else if (i == ':')
    {
      type1 = PF_INET6;
      break;
    }
  }
  for (char i : addr2)
  {
    if (i == '.')
    {
      type2 = PF_INET;
      break;
    }
    else if (i == ':')
    {
      type2 = PF_INET6;
      break;
    }
  }
  if (type1 == PF_INET && type2 == PF_INET)
  {
    return 1;
  }
  else if (type1 == PF_INET6 && type2 == PF_INET6 || type1 == PF_INET && type2 == PF_INET6)
  {
    return 2;
  }
  else
    return 3;
};

void udp_socket::print_output()
{
  for (auto i : out)
  {
    i.print_state();
  }
}

void output_udp::print_state()
{
  if (open_closed == 1)
  {
    std::cout << ip << " " << port << " udp closed" << std::endl;
  }
  else
  {
    std::cout << ip << " " << port << " udp open" << std::endl;
  }
}