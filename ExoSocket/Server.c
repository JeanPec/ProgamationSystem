#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(){

  char buffer[100];
  int sd,s,backlog,client;
  struct sockaddr_in sv,cli; // IPv4 socket, server
  socklen_t len;
  sprintf(buffer,"Hello here is my message");
	sd=socket(AF_INET, SOCK_STREAM, 0);
	if(sd==-1)
	  return -1;
	sv.sin_family = AF_INET;
	sv.sin_addr.s_addr = INADDR_ANY;
	//INADDR_ANY allows your program to work without knowing the
	//IP adress of the machine working on
	sv.sin_port = 7777;
	if(bind(sd, (struct sockaddr*)&sv,sizeof(sv)) !=0)
	  return -2;
	backlog=5;
	if(listen(sd,backlog)!=0)
	  return -3;
	while(1) {
	  len = sizeof(cli);
	  client=accept(sd,(struct sockaddr*)&cli,&len);
	  if(s==-1)
	    return -4;
	  write(client,buffer, strlen(buffer)+1);
	  close(client);
	}
	
	
	return 0;
}
