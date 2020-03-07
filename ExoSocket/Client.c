#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


int main(int argc, char* argv[]){

  int sd,s,backlog;
  struct sockaddr_in sv,cli; // IPv4 socket, server
  //socklen_t len;
  char buffer[100];
  sd=socket(AF_INET, SOCK_STREAM, 0);
  if(sd==-1)
    return -1;
  sv.sin_family = AF_INET;
  sv.sin_addr.s_addr = inet_addr(argv[1]);
  // the inet_addr function converts the specified string,
  // in the Internet standard fot notation
  // to an integer value suitable fo use as an Internet adress

  sv.sin_port = 7777;
  if(connect(sd,(struct sockaddr *)&sv,sizeof(sv)))
     return -2;
  read(sd, buffer,100);
  printf("%s\n",buffer);
    
       return 0;
}
