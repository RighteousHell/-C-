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




getchar();
  close (accepter);
  close(sock_desk);
  return 0;
}
