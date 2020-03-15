#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


#define MAX_NAME_SZ 100

void INThandler(int);
void readMessage(int ,char *);
void writeMessage(int );
void utilisateur(int, char *); //process fils pour chaque client

int main(){

  char buffer[MAX_NAME_SZ], nameUser[MAX_NAME_SZ];
  int sd,s,backlog,client,pidRead,pidWrite,pidUser,status;
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
	  printf("Client \n");
	  pidUser=fork();
	  if(pidUser==0)
	  	utilisateur(client,nameUser);
	  waitpid(pidUser,NULL,WUNTRACED);//attends la fin de l'uitlisateur
	}
	//exit(0);
	return 0;
}

void readMessage(int client, char nameClient[]){
  while(1){
    char buffer[MAX_NAME_SZ];
    if(read(client, buffer,MAX_NAME_SZ))
    	printf("\n%s : %s",nameClient,buffer);  //read error free
    else
      {
      	printf("\n%s Quit",nameClient);//Stop the client
		break;
		}
    }
	exit(1);
  }

  void writeMessage(int client){
	char buffer[MAX_NAME_SZ];
	  while(1){
		//printf("PLease Write \n"); // Debug
	    fgets(buffer, MAX_NAME_SZ, stdin);
	    if(strcmp(buffer,"quit\n")==0)
	      break;
		  //printf("Message Sent \n"); // Debug
	    write(client,buffer,strlen(buffer)+1);
	  }
	  exit(2);
  }
  /** process pour chaque client **/

  void utilisateur(int client, char nameUser[]){
	  int status,pidRead,pidWrite;
	  char buffer[MAX_NAME_SZ];
	  write(client,buffer, strlen(buffer)+1);//first mesage to the client
	  read(client,buffer,MAX_NAME_SZ);//get the name of te client
	  sprintf(nameUser,"%s",buffer);
	printf("%s connected\n",nameUser);
	  pidRead=fork();//child read and display and parent send what he gets in input
	  if(pidRead==0)
	    readMessage(client,nameUser);
	//printf("Client Start\n"); // Debug
	//create second child for write message
	pidWrite=fork();//child read and display and parent send what he gets in input
	if(pidWrite==0)
		writeMessage(client);
	do
	{
		wait(&status);
		status=WEXITSTATUS(status);
	}while(status==0);//wait until one of the process pidWrite or pidRead is finished
	close(client);
	if(status==1)
		kill(pidWrite,SIGKILL);//close Write since Read is finished
	if(status==2)
		kill(pidRead,SIGKILL);//close Read since Write is finished	
	exit(0);
	}

void INThandler(int sig){
  if(sig ==0)//only display when there is the main program who quit
  	printf("\nBye\n");
  kill(0,SIGKILL);
}
  
