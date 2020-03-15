#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_NAME_SZ 100

void child(int sd);
void INThandler(int sig);

int main(int argc, char* argv[]){

  int sd,s,backlog,pid;
  struct sockaddr_in sv,cli; // IPv4 socket, server
  //socklen_t len;
  //redefine ctrl+c
  signal(SIGINT, INThandler);
  char buffer[MAX_NAME_SZ];
  if(argc != 2)
    {fprintf(stderr,"Error argument\n"); return -1;}
  sd=socket(AF_INET, SOCK_STREAM, 0);
  if(sd==-1)
    {printf("bug 1");return -1;}
  sv.sin_family = AF_INET;
  sv.sin_addr.s_addr = inet_addr(argv[1]);
  // the inet_addr function converts the specified string,
  // in the Internet standard fot notation
  // to an integer value suitable fo use as an Internet adress

  sv.sin_port = 7777;
  printf("Try to connect \n");
  if(connect(sd,(struct sockaddr *)&sv,sizeof(sv)))
     {printf("bug 2");return -2;}
  printf("Connected \n");
  read(sd, buffer,MAX_NAME_SZ);
  printf("%s\n",buffer);
  printf("What's your name ?\n");
  fgets(buffer, MAX_NAME_SZ, stdin);
  strtok(buffer,"\n");// read without \n 
  write(sd,buffer,strlen(buffer)+1);
  pid=fork();//parent read and child write what he receives
  if(pid==0)
    child(sd);
  while(1){
    fgets(buffer, MAX_NAME_SZ, stdin);
    if(strcmp(buffer,"quit\n")==0)
      break;
    write(sd,buffer,strlen(buffer)+1);
    }
  close(sd);
  kill(pid,SIGKILL);//close the child process
  exit(0);
       return 0;
}


void child(int sd){
  while(1){
    char buffer[MAX_NAME_SZ];
    if(!read(sd, buffer,MAX_NAME_SZ))
      {
	printf("\nServer Stopped");
  close(sd);
	INThandler(0);//clean
      }
    else
      printf("\n%s",buffer);
  }
}

void INThandler(int sig){
  if(sig ==0)//only display when there is the main program who quit
  	printf("\nBye\n");
  kill(0,SIGKILL);
}
