#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <vector>
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <fstream>
#include <sstream>
#include <sys/time.h>
#include <cstdlib>
#include <unistd.h>
#include <mbedtls/md5.h>                   //33+16=49+4381
#include <chrono>
#include <thread>
#define CS_SIZE 16
#define TIME_SIZE 21

#pragma pack (push, 1)
struct packet
{
  uint32_t packet_size;
  uint32_t sequence_number;
  char time [TIME_SIZE] = {'\0'};
  uint32_t data_size;
  std::string  data;
  unsigned char cs_md5[CS_SIZE] = {'\0'};
};
#pragma pack (pop)

void fill_packet_data (packet& pct, std::ifstream& stream)
{
 if (!pct.data.empty())
 {
   pct.data.clear();
 }
getline(stream, pct.data);
pct.data.shrink_to_fit();
pct.data_size = pct.data.length();
}


void fill_packet_time(packet& pct)
{
std::string time;
timeval time_val;
gettimeofday(&time_val, NULL);
tm* tmp_tm = localtime(&time_val.tv_sec);
time += std::to_string(tmp_tm->tm_hour) + ":" + std::to_string(tmp_tm->tm_min) + ":" + std::to_string(tmp_tm->tm_sec) + ":" + std::to_string(time_val.tv_usec/1000) + "ms" + " +" + std::to_string(time_val.tv_usec%1000) + "mcs";
strcpy(pct.time, time.c_str());
}

void  make_cs_md5 (packet& pct)
{
mbedtls_md5 (reinterpret_cast<const unsigned char*>(pct.data.c_str()), size_t(pct.data_size), pct.cs_md5);
}

inline void sequence_increase(packet& pct)
{
++pct.sequence_number;
}

inline void sequence_set_to_null(packet& pct)
{
pct.sequence_number = 0;
}

void send_msg (packet& pct, int socket)
{
sequence_increase(pct);
fill_packet_time(pct);

int res = send (socket, (void*) &pct, pct.packet_size, 0);
std::cout << res << " bites was sent" << std::endl;
std::cout << "Sent:packet #" << pct.sequence_number <<" time of send: " << pct.time << std::endl;
}
