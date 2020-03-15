#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>


#include "listClient.h"


#define MAX_NAME_SZ 100
#define PIPE_ADD 1
#define PIPE_REMOVE 2
#define PIPE_SEND 3
#define PIPE_CONFIRM 4
#define PIPE_DISPLAY 5

void INThandler(int);
void readMessage(int ,char *);
void writeMessage(int );
void utilisateur(int, char *,int *,int*); //process fils pour chaque client
void server(int *,int [][2]);//gere tous les utilisateurs
void server_control(int *);

/*
Problème restant 
	faire un tableau de pipe statique implique que si un utilisteur quitte le pipe sera inutilisé
	Il y aura un pipe qui ne sera pas utilisé
	dans ce cas j'ai supposé quand je veux ecrire a tous les utilisateur je fais une boucle selon le nombre d'utilisateur
	si un utilistaeur quitte et un autre rejoint il y aura x utilisteurs mais x+1 pipe utilisé
	ma boucle n'ira que jusqu'à la pipe x
	pour regler le problème temporairement j'ai enlevé la decrementation du nombre d'utilisateur ce n'est qu'une solution temporaire
*/


int main(){

  char buffer[MAX_NAME_SZ], nameUser[MAX_NAME_SZ];
  int sd,backlog,client,i;
  int fd1[2];
  int fd2[MAX_NAME_SZ][2];
  struct sockaddr_in sv,cli; // IPv4 socket, server
  socklen_t len;
  
  // pipe to communicate
  if(pipe(fd1) == -1)
		exit(1);
  for(i=0;i<MAX_NAME_SZ;i++)
	pipe(fd2[i]);
  

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
	backlog=10;
	if(listen(sd,backlog)!=0)
	  {printf("bug 3");return -1;}
	if(fork()==0)
		server_control(fd1);//console pour gérer le serveur 
	if(fork()==0)
		server(fd1,fd2);//serveur va gerer les clients
	//parent pere entre dans boucle infini pour acceuilir les clients
	i=0;
	while(1) {
	  len = sizeof(cli);
	  client=accept(sd,(struct sockaddr*)&cli,&len);//accepte liaison avec le client
	  if(client==-1)
	    {printf("bug 4");return -1;}
	  if(fork()==0)
	  	utilisateur(client,nameUser,fd1,fd2[i]);
	i++;
	}
	//exit(0);
	return 0;
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
	  printf("\nexit(2)");
	  exit(2);
  }
  /** process pour chaque client **/

  void utilisateur(int client, char nameUser[],int *fd1,int *fd2){
	  char buffer[MAX_NAME_SZ],sendbuffer[MAX_NAME_SZ];
	  /*ecrit dans fd1 */
	  close(fd1[0]);
	  close(fd2[1]);

	  write(client,"Serveur Sylvain", strlen("Serveur Sylvain")+1);//first mesage to the client
	  read(client,buffer,MAX_NAME_SZ);//get the name of the client
	  sprintf(nameUser,"%s",buffer);
	//printf("%s connected\n",nameUser);
	sprintf(buffer,"%d,%s,%d",PIPE_ADD,nameUser,client);//ecrit les infos pour remplir la liste 
	write(fd1[1],buffer,100);//info pour ajouter l'utilisteur
	if(fork()==0)
	{
		while(1){
		char buffer[MAX_NAME_SZ],bufferPipe[MAX_NAME_SZ];
		char *strToken;
		int clienttest;
		read(fd2[0],buffer,MAX_NAME_SZ);//lit message envoyé par le serveur
		//sprintf(bufferPipe,"%d,%d,message sent",PIPE_CONFIRM,client);
		//write(fd1[1],bufferPipe,strlen(bufferPipe)+1);
		strToken= strtok(buffer,",");//coupe le message selon ,
		clienttest=atoi(strToken);//si la valeur est égale au fichier du process on n'affichera pas le message
		if(client!=clienttest){
			strToken=strtok(NULL,",");//recupère token suivant
			strcpy(bufferPipe, strToken);//recupère le message à envoyer
			//printf("\n message envoyée %s, %d",bufferPipe,clienttest);
			write(client,bufferPipe,strlen(bufferPipe)+1);
		}
		
    	}
	}
	while(1){
		char buffer[MAX_NAME_SZ];
		if(read(client, buffer,MAX_NAME_SZ)){
			sprintf(sendbuffer,"%d,%s,%d",PIPE_SEND,buffer,client);//utilise nameUser pour y mettre le mesage
			write(fd1[1],sendbuffer,MAX_NAME_SZ);
		}
		else
		{
			sprintf(buffer,"%d,%s,%d",PIPE_REMOVE,nameUser,client);//ecrit les infos pour remplir la liste 
			write(fd1[1],buffer,MAX_NAME_SZ);
			break;
			}
    }
	exit(0);
	}

void server(int *fd1,int fd2[][2]){
	Liste *liste;//list of the pid and name of the client
	int sizeListe,test,client,i;//size of the list
	char buffer[MAX_NAME_SZ],bufferPipe[MAX_NAME_SZ],userName[MAX_NAME_SZ];
	char * strToken;
	close(fd1[1]);//lit dans le pipe fd1
	liste=initialisation(&sizeListe);
	while(1){//attends que les utilisateurs remplissent leur nom et envoir leur message
		read(fd1[0],buffer,MAX_NAME_SZ);
		strToken= strtok(buffer,",");//coupe le message selon ,
		test=atoi(strToken);//recupère la valuer test qui permets de savoir quel action faire
		strToken=strtok(NULL,",");//recupère token suivant
		strcpy(bufferPipe, strToken);//recupère le message ou le nom du client
		strToken=strtok(NULL,",");
		client=atoi(strToken);//recupère le fichier descripteur du client
		switch(test)
		{
		case PIPE_ADD:
			ajoutDebut(liste,bufferPipe,client,&sizeListe);
			break;
		case PIPE_REMOVE:
			sprintf(buffer,"%d,%s :> left \n",client,bufferPipe);
			for(i=0;i<sizeListe;i++)
			{
				write(fd2[i][1],buffer,strlen(buffer)+1);
			}
			removeAtNum(liste,client,&sizeListe);
			//here bufferPipe contains the name of the use who left
			
			break;
		case PIPE_SEND:
			getName(liste,client,userName);
			sprintf(buffer,"%d,%s :> %s \n",client,userName,bufferPipe);
			for(i=0;i<sizeListe;i++)
			{
				write(fd2[i][1],buffer,strlen(buffer)+1);
			}
			break;
		case PIPE_DISPLAY:
			afficherListe(liste);
			break;
	}
	}

}


void server_control(int *fd){
	char buffer[MAX_NAME_SZ];
	close(fd[0]);//write in the pipe

	printf("Console du Serveur \n");
	printf("Commande disponible afficher : quit \n");
	while(1){
		fgets(buffer,MAX_NAME_SZ,stdin);
		if(!strcmp(buffer,"afficher\n"))
		{
			sprintf(buffer,"%d, ,0",PIPE_DISPLAY);
			write(fd[1],buffer,strlen(buffer)+1);
		}	
		if(!strcmp(buffer,"quit\n"))
		{
			INThandler(0);
		}	
	}

}

void INThandler(int sig){
if(sig ==0)//only display when there is the main program who quit
  	printf("\nBye\n");
  kill(0,SIGKILL);
}
  
