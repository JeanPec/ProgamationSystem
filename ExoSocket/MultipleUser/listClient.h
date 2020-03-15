#ifndef LISTECLIENT_H
#define LISTECLIENT_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//structure

typedef struct Element Element;
struct Element
{
    int numbClient;
    char nomClient[50];
    Element *suivant;
};

typedef struct Liste Liste;
struct Liste
{
    Element *premier;
};


//functions

//initialise la liste
Liste *initialisation(int*);
//ajoute un élement en début de liste
void ajoutDebut(Liste *,char *, int ,int*);
//supprime le premier élement de la liste
void removeBegin(Liste *,int*);
//supprime à l'élement dont le numéro est donnée
void removeAtNum(Liste *, int,int*);
//affiche la liste
void afficherListe(Liste *);
//supprimer un a un les elements de la liste
void delete(Liste *,int*);
//recuperer le nom enfonction du numéro du client
void getName(Liste *,int ,char*);

#endif
