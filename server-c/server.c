// A standard TCP/IP Server that sends 64KB data constantly to a single client
// until the user closes the Android app.
// by Jeff Sasaki

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define BUFSIZE  65536
#define LISTENQ  20

int main(int argc, char *argv[]) {
  int                 listenfd, connfd;
  char                buffer[BUFSIZE];
  short               port;
  struct sockaddr_in  serveraddr;
  int count;
  count = 0;

  // You must specify a port number as a command line argument
  if (argc != 2) {
    fprintf(stderr, "usage: %s port\n", argv[0]);
    return 1;
  }
  if (sscanf(argv[1], "%hu", &port) != 1) {
    fprintf(stderr, "invalid port: %s\n", argv[1]);
    return 2;
  }

  if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    return 3;
  }

  memset(&serveraddr, 0, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_port = htons(port);
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(listenfd, (const struct sockaddr *)&serveraddr, sizeof(serveraddr))
      == -1) {
    perror("bind");
    return 4;
  }

  if (listen(listenfd, LISTENQ) == -1) {
    perror("listen");
    return 5;
  }
  printf("Server Start\n");
  while (1) {
    if ((connfd = accept(listenfd, 0, 0)) == -1) {
      perror("accept");
      return 6;
    }

    while(1)
    {
      sprintf(buffer, "Count = %d\n", count);
      if (write(connfd, buffer, strlen(buffer)) == -1)
      {
        perror("write");
        return 1;
      }
      printf("Sending %d Bytes... Count = %d\n", (int)sizeof(buffer), count);
      count++;
      //usleep(100000); //sleep for 100ms
    }
    if (close(connfd) == -1) {
      perror("close");
      return 8;
    }
  }
  return 0;
}
