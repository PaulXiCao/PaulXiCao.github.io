#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main()
{
  const int clientSocket = socket(PF_INET, SOCK_STREAM, 0);

  struct sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(12345);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));

  socklen_t addr_size = sizeof serverAddr;
  connect(clientSocket, (struct sockaddr *)&serverAddr, addr_size);

  char buffer[1024];
  recv(clientSocket, buffer, sizeof(buffer), 0);
  printf("Data received: %s", buffer);

  return 0;
}
