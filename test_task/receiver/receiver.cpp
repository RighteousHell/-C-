#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <chrono>
#include <thread>
#include <sstream>
#include <mbedtls/md5.h>
#include <sys/time.h>

#define CS_SIZE 16
#define TIME_SIZE 25


#pragma pack (push, 1)
struct packet
{
  uint16_t packet_size;
  uint16_t sequence_number = 0;
  char time [TIME_SIZE] = {'\0'};
  uint16_t data_size;
  unsigned char cs_md5[CS_SIZE] = {'\0'};
  std::string  data;

};
#pragma pack (pop)

std::string calculate_receive_time();
std::string compare_cs_md5 (packet& pct);
void receive_msg (packet& pct, int accepter);

int main (int argc, char const *argv[])
{

  if (argc != 2)
  {
    std::cout << "Correct form is: receiver [port]" << std::endl;
    return -1;
  }

    struct sockaddr_in addr_receiver, addr_sender;

    memset(&addr_receiver, 0, sizeof(addr_receiver));
    addr_receiver.sin_family = AF_INET;
    addr_receiver.sin_port = htons(atoi(argv[1]));
    addr_receiver.sin_addr.s_addr = INADDR_ANY;

    int sock_desk = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_desk < 0)
    {
      std::cout << "Socket opening: something gone wrong!" << std::endl;
      return -1;
    }

    if ( bind (sock_desk,  (struct sockaddr*) &addr_receiver, sizeof(addr_receiver)) < 0)
    {
      std::cout << "Socket binding: something gone wrong!" << std::endl;
      return -1;
    }

    listen(sock_desk,1);
    socklen_t sender_len = sizeof(addr_sender);
    int accepter = accept(sock_desk, (struct sockaddr*) &addr_sender, &sender_len);

    if ( accepter < 0)
    {
      std::cout << "Sender accepting: something gone wrong!" << std::endl;
      return -1;
    }

packet pack;

while(pack.sequence_number < 200)
{
  receive_msg (pack, accepter);
}

  close (accepter);
  close(sock_desk);
  return 0;
}

void  receive_msg (packet& pct, int accepter)
{
recv (accepter, &pct.packet_size, sizeof(uint16_t), 0);
recv (accepter, &pct.sequence_number, sizeof(uint16_t), 0);
recv (accepter, &pct.time, TIME_SIZE, 0);
recv (accepter, &pct.data_size, sizeof(uint16_t), 0);
recv (accepter, &pct.cs_md5, CS_SIZE, 0);
char* buf = new char [pct.data_size];
int res = recv (accepter, buf, pct.data_size, 0);
pct.data = buf;
delete [] buf;
if (res)
{
std::cout << "Received: packet#" << pct.sequence_number << " receive time: " << calculate_receive_time() << " CS_MD5: " << compare_cs_md5(pct) << std::endl;
}
}

std::string calculate_receive_time()
{
std::string time;
timeval time_val;
gettimeofday(&time_val, NULL);
tm* tmp_tm = localtime(&time_val.tv_sec);
time += std::to_string(tmp_tm->tm_hour) + ":" + std::to_string(tmp_tm->tm_min) + ":" + std::to_string(tmp_tm->tm_sec) + ":" + std::to_string(time_val.tv_usec/1000) + "ms" + " +" + std::to_string(time_val.tv_usec%1000) + "mcs";
return time;
}

std::string compare_cs_md5 (packet& pct)
{
unsigned char tmp_md5 [CS_SIZE];
mbedtls_md5 (reinterpret_cast<const unsigned char*>(pct.data.c_str()), size_t(pct.data_size), tmp_md5);
for (short i=0; i< CS_SIZE; i++)
{
  if (tmp_md5[i] != pct.cs_md5[i])
  {
    return "FAIL";
  }
}
return "PASS";

}
