#pragma once

#include <stdio.h>
#include <stdlib.h>

typedef struct MatriceAdjacence	// Structure contenant la matrice d'adjacence
{
	int taille;
	int** matrice;
	int** visite;
} MatriceAdjacence;

typedef struct ListeSuccesseurs
{
	int id;
	int* succ;
	int nbSucc;
}ListeSuccesseurs;

typedef struct MatriceSuccesseurs
{
	ListeSuccesseurs* liste;
	struct MatriceSuccesseurs* next;
}MatriceSuccesseurs;

MatriceAdjacence* GetMatriceFromFile(char* pathFile);	// Fonction de lecture d'une matrice d'adjacence

void PrintMatriceAdjacence(MatriceAdjacence* mat);		// Affichage d'une matrice d'adjacence

int NumberWayWithLength(MatriceAdjacence* mat, int longueur);

int IsLengthBetweenPoint(MatriceAdjacence* mat, int origine, int extremite, int longueurSouhaite);

ListeSuccesseurs* GetElementSuccessors(MatriceAdjacence* mat, int id);

void PrintListeSuccesseur(ListeSuccesseurs* list);

void CreateMatriceWithUserInput();

void AddSuccessor(ListeSuccesseurs* liste, int successor);

void PrintMatriceSuccesseurs(MatriceSuccesseurs* mat);

void freeMatriceSuccesseurs(MatriceSuccesseurs* mat);

void freeListeSuccesseur(ListeSuccesseurs* list);

void freeMatriceAdjacence(MatriceAdjacence* matriceAdj);

void WriteMatriceInFile(char* path, MatriceSuccesseurs* mat);

void PrintSuffixeMatrice(MatriceAdjacence* mat);
void _PrintSuffixeMatrice(MatriceAdjacence* mat, int racine, int prof);

int* Connexite(MatriceAdjacence* mat);