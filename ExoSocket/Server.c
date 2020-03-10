#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


#define MAX_NAME_SZ 100

void INThandler(int sig);
void readMessage(int client);
void writeMessage(int client);

int main(){

  char buffer[MAX_NAME_SZ];
  int sd,s,backlog,client,pidRead,pidWrite,status;
  struct sockaddr_in sv,cli; // IPv4 socket, server
  socklen_t len;
  //redefine ctrl+c
  signal(SIGINT, INThandler);
  
  sprintf(buffer,"Server Sylvain");
	sd=socket(AF_INET, SOCK_STREAM, 0);
	if(sd==-1)
	  {printf("bug 1");return -1;}
	
	sv.sin_family = AF_INET;
	sv.sin_addr.s_addr = INADDR_ANY;
	//INADDR_ANY allows your program to work without knowing the
	//IP adress of the machine working on
	sv.sin_port = 7777;
	if(bind(sd, (struct sockaddr*)&sv,sizeof(sv)) !=0)
	  {printf("bug 2");return -1;}
	backlog=5;
	if(listen(sd,backlog)!=0)
	  {printf("bug 3");return -1;}
	
	while(1) {
	  len = sizeof(cli);
	  client=accept(sd,(struct sockaddr*)&cli,&len);
	  if(s==-1)
	    {printf("bug 4");return -1;}
	  write(client,buffer, strlen(buffer)+1);
	//printf("Client Start\n");
	  pidRead=fork();//child read and display and parent send what he gets in input
	  if(pidRead==0)
	    readMessage(client);
	printf("Client Start\n");
	//create second child for write message
	pidWrite=fork();//child read and display and parent send what he gets in input
	if(pidWrite==0)
		writeMessage(client);
	/*waitpid(pidWrite,&status,WUNTRACED);//wait for process read or wrtie to be finished
	waitpid(pidRead,&status,WUNTRACED);*/
	do
	{
		wait(&status);
		status=WEXITSTATUS(status);
	}while(status==0);//wait until one of the process pidWrite or pidRead is finished
	close(client);
	printf("\nStatus %d\n",status);
	if(status==1)
		kill(pidWrite,SIGKILL);//close Write since Read is finished
	if(status==2)
		kill(pidRead,SIGKILL);//close Read since Write is finished	
	//printf("Client terminated\n");
	}
	//exit(0);
	return 0;
}

void readMessage(int client){
  while(1){
    char buffer[MAX_NAME_SZ];
    if(read(client, buffer,MAX_NAME_SZ))
    	printf("\nclient : %s",buffer);  //read error free
    else
      {
      	printf("\nClient Quit");//Stop the client
		break;
		}
    }
	printf("\nexit(1)");
	exit(1);
  }

  void writeMessage(int client){
	char buffer[MAX_NAME_SZ];
	  while(1){
		//printf("PLease Write \n");
	    fgets(buffer, MAX_NAME_SZ, stdin);
	    if(strcmp(buffer,"quit\n")==0)
	      break;
		  //printf("Message Sent \n");
	    write(client,buffer,strlen(buffer)+1);
	  }
	  printf("\nexit(2)");
	  exit(2);
  }

void INThandler(int sig){
  printf("\nBye\n");
  kill(0,SIGKILL);
}
  
