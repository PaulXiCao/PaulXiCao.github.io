#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

int main()
{
  const int welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);

  struct sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(12345);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));

  bind(welcomeSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

  if (listen(welcomeSocket, 5) == 0)
    printf("Listening\n");
  else
  {
    printf("Error\n");
    exit(7);
  }

  struct sockaddr_storage serverStorage;
  socklen_t addr_size = sizeof serverStorage;
  const int newSocket = accept(welcomeSocket, (struct sockaddr *)&serverStorage, &addr_size);

  const char msg[] = "Hello World\n";
  send(newSocket, msg, sizeof(msg), 0);

  return 0;
}
