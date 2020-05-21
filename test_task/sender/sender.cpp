#include "sender.h"


int  main(int argc, char const *argv[])
{

  if (argc != 3)
  {
    std::cout << "Correct form is: sender [ip_addr] [port]" << std::endl;
    return -1;
  }


  int port = atoi(argv[2]);
  struct sockaddr_in addr_receiver;
  addr_receiver.sin_family = AF_INET;
  addr_receiver.sin_port = htons(atoi(argv[2]));
  addr_receiver.sin_addr.s_addr = inet_addr(argv[1]);
  inet_pton(AF_INET, argv[1], &addr_receiver.sin_addr.s_addr);


int sock_desk = socket(AF_INET, SOCK_STREAM, 0);
if (sock_desk < 0)
{
  std::cout << "Socket opening: something gone wrong!" << std::endl;
  return -1;
}


if (connect(sock_desk, (sockaddr*) &addr_receiver, sizeof(addr_receiver)) < 0)
{

  std::cout << "Connecting: something gone wrong!" << std::endl;
  return -1;
}


packet pack;
std::ifstream inp_str ("sender/source.csv");

/***** General cycle*****/
for (int i = 0; i < 2; i++)
{
  std::ifstream inp_str ("sender/source.csv");
  if (!inp_str.is_open())
  {
      std::cout << "Opening ifstream: something gone wrong!" << std::endl;
  }
  for (int j = 0; j < 100; j++)
  {
  fill_packet_data(pack, inp_str);
  make_cs_md5(pack);
  pack.packet_size = 47 + pack.data_size;
  send_msg (pack, sock_desk);
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  //sequence_set_to_null(pack);
  inp_str.close();
  if (i < 1)
  {
  sleep(10);
  }
}


close(sock_desk);
return 0;
}
