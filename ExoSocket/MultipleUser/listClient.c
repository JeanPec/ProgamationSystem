#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "listClient.h"

/*
int main(){
    Liste *liste,*liste2;
    int size1,size2;
    liste = initialisation(&size1);
    liste2 = initialisation(&size2);
    ajoutDebut(liste,"A",101,&size1);
    ajoutDebut(liste,"B",202,&size1);
    ajoutDebut(liste,"C",303,&size1);
    ajoutDebut(liste,"D",404,&size1);
    ajoutDebut(liste,"E",505,&size1);
    ajoutDebut(liste,"F",606,&size1);
    removeAtNum(liste,202,&size1);
    afficherListe(liste);
    printf("%d \n",size1);
    removeAtNum(liste,101,&size1);
    afficherListe(liste);
    printf("%d \n",size1);
    removeAtNum(liste,606,&size1);
    afficherListe(liste);
    printf("%d \n",size1);
    delete(liste,&size1);
    printf("%d \n",size1);
    //ajoutDebut(liste,"Mathieu",303);
    return 0;
}
*/
Liste *initialisation(int* sizeListe)
{
    Liste *liste = malloc(sizeof(Liste));//aloue avec la taille de liste
    Element *element = malloc(sizeof(Element));

    if (liste == NULL || element == NULL)
    {
        exit(EXIT_FAILURE);
    }

    sprintf(element->nomClient,"%s","");
    element->numbClient=0;
    element->suivant = NULL;
    liste->premier = element;
    *sizeListe = 0;

    return liste;
}

void ajoutDebut(Liste *liste,char *nom, int num,int *sizeListe){
    /*creation nouvel élement*/
    Element *element = malloc(sizeof(Element));
    if (liste == NULL || element == NULL)
    {
        exit(EXIT_FAILURE);
    }
    sprintf(element->nomClient,"%s",nom);
    element->numbClient=num;
    element->suivant = liste->premier;//nouvel élement pointe vers l'ancien premier élement
    liste->premier=element;//nouvel élement est le premier de la liste
    *sizeListe +=1;
}


void removeBegin(Liste *liste,int* sizeListe){
    if (liste == NULL)
    {
        exit(EXIT_FAILURE);
    }

    if (liste->premier != NULL) //vérifie que la liste n'est pas vide
    {
        Element *aSupprimer = liste->premier;
        liste->premier = liste->premier->suivant;
        *sizeListe -=1;
        free(aSupprimer);
    }
}

void removeAtNum(Liste *liste, int num,int *sizeListe){
    if(liste == NULL)
    {
        exit(EXIT_FAILURE);
    }

    Element *actuel = liste->premier;//recupere premier élement
    Element *precedent = NULL;
    while(actuel!=NULL){//travers toute la liste
        if(actuel->numbClient==num){//rencontre l'élement à supprimer
            if(precedent==NULL)//premier élement de la liste
                liste->premier = liste->premier->suivant;
            else if(actuel->suivant==NULL)//dernier élement de la liste
                precedent->suivant=NULL;
            else 
                precedent->suivant=actuel->suivant;//recupere l'adresse du suivant
            free(actuel);
            //*sizeListe -=1;
            break;
        }
        precedent=actuel;//stock pour avoir l'élement avant actuel
        actuel=actuel->suivant;
    }
}

void afficherListe(Liste *liste)
{
    if(liste == NULL)
    {
        exit(EXIT_FAILURE);
    }

    Element *actuel = liste->premier;

    while(actuel != NULL)
    {
        printf(" |%d : %s| ",actuel->numbClient,actuel->nomClient);
        actuel = actuel->suivant;
    }
    printf("NULL\n");
}

void delete(Liste *liste,int *sizeListe){
    if(liste == NULL)
    {
        exit(EXIT_FAILURE);
    }

    Element *actuel = liste->premier;
    Element *aSupprimer = liste->premier;
    while(actuel != NULL)
    {
        aSupprimer=actuel;
        actuel=actuel->suivant;//recupere le prochain
        free(aSupprimer);
    }
    *sizeListe =0;
}

void getName(Liste *liste, int num,char *userName){
    if(liste == NULL)
    {
        exit(EXIT_FAILURE);
    }

    Element *actuel = liste->premier;//recupere premier élement
    while(actuel!=NULL){//travers toute la liste
        if(actuel->numbClient==num){//rencontre l'élement à supprimer
            strcpy(userName,actuel->nomClient);//recupere le nom
            break;
        }
        actuel=actuel->suivant;//passe a lelement suivant
    }
}




