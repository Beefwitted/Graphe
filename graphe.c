#include "graphe.h"

MatriceAdjacence* GetMatriceFromFile(char* pathFile)
{
	FILE* fichier;
	errno_t err = fopen_s(&fichier, pathFile, "r");
	if (err)
	{
		printf("Impossible d'ouvrir le fichier %s (erreur code : %d)\n", pathFile, err);
		return NULL;
	}

	if (fichier)
	{
		MatriceAdjacence* mat = malloc(sizeof(MatriceAdjacence));
		if (mat == NULL)
		{
			fclose(fichier);
			printf("Impossible d'allouer de la mémoire pour la matrice d'adjacence\n");
			return NULL;
		}
		err = fscanf_s(fichier, "%d", &(mat->taille));
		if (err == EOF)
		{
			printf("Une erreur a ete rencontre lors de la lecture de la taille de la matrice (erreur code : %d)\n", err);
			free(mat);
			fclose(fichier);
			return NULL;
		}
		else
		{
			mat->matrice = malloc(sizeof(int*) * mat->taille);
			mat->visite = malloc(sizeof(int*) * mat->taille);
			if (mat->matrice)
			{
				for (int i = 0; i < mat->taille; i++)
				{
					mat->matrice[i] = malloc(sizeof(int) * mat->taille);
					mat->visite[i] = malloc(sizeof(int) * mat->taille);
					if (!mat->matrice[i])
					{
						printf("Impossible d'allouer de la mémoire pour la matrice\n");
						fclose(fichier);
						freeMatriceAdjacence(mat);
						return NULL;
					}
				}
			}
			else
			{
				printf("Impossible d'allouer de la mémoire pour la matrice\n");
				free(mat);
				return NULL;
			}

			for (int x = 0; x < mat->taille; x++)
			{
				for (int y = 0; y < mat->taille; y++)
				{
					mat->matrice[x][y] = 0;
				}
			}

			char c;
			int nb1, nb2;
			fscanf_s(fichier, "%c", &c, 1);

			while (err != EOF)
			{
				fscanf_s(fichier, "%d", &nb1);
				fscanf_s(fichier, "%c", &c, 1);
				err = fscanf_s(fichier, "%d", &nb2);

				if (err != EOF)
				{
					mat->matrice[nb1][nb2] = 1;
				}

				err = fscanf_s(fichier, "%c", &c, 1);
			}
		}

		fclose(fichier);

		return mat;
	}

	return NULL;
}

void PrintMatriceAdjacence(MatriceAdjacence* mat)
{
	for (int x = 0; x < mat->taille; x++)
	{
		printf("| ");
		for (int y = 0; y < mat->taille; y++)
		{
			printf("%d ", mat->matrice[x][y]);
		}
		printf("|\n");
	}
}

int NumberWayWithLength(MatriceAdjacence* mat, int longueur)
{
	int cp = 0;

	for (int origine = 0; origine < mat->taille; origine++)
	{
		for (int extremite = 0; extremite < mat->taille; extremite++)
		{
			cp += IsLengthBetweenPoint(mat, origine, extremite, longueur);
		}
	}

	return cp;
}

int IsLengthBetweenPoint(MatriceAdjacence* mat, int origine, int extremite, int longueurSouhaite)
{
	int nbChemin = 0;
	for (int extr = 0; extr < mat->taille; extr++)
	{
		if (mat->matrice[origine][extr] == 1)
		{
			if (extr == extremite && longueurSouhaite == 1)
			{
				nbChemin++;
			}
			else if (longueurSouhaite > 1)
				nbChemin += IsLengthBetweenPoint(mat, extr, extremite, longueurSouhaite-1);
		}
	}

	return nbChemin;
}

ListeSuccesseurs* GetElementSuccessors(MatriceAdjacence* mat, int id)
{
	ListeSuccesseurs* liste = (ListeSuccesseurs*)malloc(sizeof(ListeSuccesseurs));
	if (!liste)
	{
		printf("Erreur lors de l'allocation de memoire pour la liste des successeurs\n");
		return NULL;
	}

	liste->id = id;
	liste->succ = NULL;
	liste->nbSucc = 0;

	for (int i = 0; i < mat->taille; i++)
	{
		if (mat->matrice[id][i] == 1)
		{
			liste->nbSucc++;
			if (!liste->succ)
			{
				liste->succ = (int*)malloc(sizeof(int));
				if (!liste->succ)
				{
					printf("Erreur lors de l'allocation de memoire pour le premier successeur\n");
					freeListeSuccesseur(liste);
					return NULL;
				}
			}
			else
			{
				int* tmp = (int*)realloc(liste->succ, sizeof(int) * liste->nbSucc);
				if (tmp)
					liste->succ = tmp;
				else
				{
					printf("Erreur lors de l'allocation de memoire pour ajouter un successeur\n");
					freeListeSuccesseur(liste);
					return NULL;
				}
			}

			liste->succ[liste->nbSucc - 1] = i;
		}
	}

	return liste;
}

void PrintListeSuccesseur(ListeSuccesseurs* list)
{
	printf("%d -> ", list->id);
	for (int i = 0; i < list->nbSucc; i++)
	{
		printf("%d / ", list->succ[i]);
	}
	printf("\n");
}

void CreateMatriceWithUserInput()
{
	printf("\n\nPour creer votre graphe vous devez renseigner ligne par ligne un sommet et son successeur suivit d'un espace (entrez tout sauf un nombre pour arreter la saisie) :\n");
	
	int ret;
	int som, succ;

	MatriceSuccesseurs* mat = NULL;
	MatriceSuccesseurs* tmp = mat, *prec;

	ret = scanf_s("%d %d", &som, &succ);
	while (ret == 2)
	{
		if (!mat)
		{
			mat = (MatriceSuccesseurs*)malloc(sizeof(MatriceSuccesseurs));
			mat->liste = (ListeSuccesseurs*)malloc(sizeof(ListeSuccesseurs));
			mat->liste->id = som;
			mat->liste->nbSucc = 0;
			mat->liste->succ = NULL;
			mat->next = NULL;
			tmp = mat;
		}
		else
		{
			int trouver = 0;
			tmp = mat;
			prec = NULL;
			while (tmp && !trouver)
			{
				if (tmp->liste->id == som)
					trouver = 1;
				else
				{
					prec = tmp;
					tmp = tmp->next;
				}
			}

			if (!trouver && prec)
			{
				if (!prec)
					prec = tmp;

				prec->next = (MatriceSuccesseurs*)malloc(sizeof(MatriceSuccesseurs));
				tmp = prec->next;
				tmp->liste = (ListeSuccesseurs*)malloc(sizeof(ListeSuccesseurs));
				tmp->liste->id = som;
				tmp->liste->nbSucc = 0;
				tmp->liste->succ = NULL;
				tmp->next = NULL;
			}
		}

		AddSuccessor(tmp->liste, succ);
		
		ret = scanf_s("%d %d", &som, &succ);
	}

	WriteMatriceInFile("monGraphe.txt", mat);

	PrintMatriceSuccesseurs(mat);
	freeMatriceSuccesseurs(mat);
}

void AddSuccessor(ListeSuccesseurs* liste, int successor)
{
	if (!liste->nbSucc)
	{
		liste->succ = (int*)malloc(sizeof(int));
	}
	else
	{
		int* tmp = (int*)realloc(liste->succ, sizeof(int) * (liste->nbSucc+1));
		if (!tmp)
		{
			printf("Erreur d'allocation lors de l'ajout d'un successeur\n");
			return;
		}

		liste->succ = tmp;
	}
	liste->succ[liste->nbSucc] = successor;
	liste->nbSucc++;
}

void PrintMatriceSuccesseurs(MatriceSuccesseurs* mat)
{
	while (mat)
	{
		PrintListeSuccesseur(mat->liste);
		mat = mat->next;
	}
}

void freeMatriceSuccesseurs(MatriceSuccesseurs* mat)
{
	if (mat)
	{
		freeListeSuccesseur(mat->liste);
		freeMatriceSuccesseurs(mat->next);
		free(mat);
	}
}

void freeListeSuccesseur(ListeSuccesseurs* list)
{
	if (list)
	{
		if (list->succ)
			free(list->succ);

		free(list);
	}
}

void freeMatriceAdjacence(MatriceAdjacence* mat)
{
	if (mat != NULL)
	{
		if (mat->matrice != NULL)
		{
			for (int i = 0; i < mat->taille; i++)
			{
				if (mat->matrice[i] != NULL)
					free(mat->matrice[i]);
				if (mat->visite[i] != NULL)
					free(mat->visite[i]);
			}
			free(mat->matrice);
		}
		free(mat);
	}
}

void WriteMatriceInFile(char* path, MatriceSuccesseurs* mat)
{
	FILE* fichier;
	errno_t err = fopen_s(&fichier, path, "w");
	if (!err && fichier)
	{
		int maxID = 0;
		MatriceSuccesseurs* tmp = mat;
		while (tmp)
		{
			for (int i = 0; i < tmp->liste->nbSucc; i++)
			{
				if (maxID < tmp->liste->id)
					maxID = tmp->liste->id;

				for (int j = 0; j < mat->liste->nbSucc; j++)
				{
					if (maxID < tmp->liste->succ[j])
						maxID = tmp->liste->succ[j];
				}
			}
			tmp = tmp->next;
		}

		fprintf_s(fichier, "%d\n", maxID+1);

		while (mat)
		{
			for (int i = 0; i < mat->liste->nbSucc; i++)
			{
				fprintf_s(fichier, "%d %d\n", mat->liste->id, mat->liste->succ[i]);
			}
			mat = mat->next;
		}

		fclose(fichier);
	}
	else
		printf("Erreur lors de l'ouvreture du fichier %s\n", path);
}

void PrintSuffixeMatrice(MatriceAdjacence* mat)
{
	for (int x = 0; x < mat->taille; x++)
	{
		for (int y = 0; y < mat->taille; y++)
		{
			mat->visite[x][y] = 0;
		}
	}

	for (int x = 0; x < mat->taille; x++)
	{
		for (int y = 0; y < mat->taille; y++)
		{
			if (mat->matrice[x][y] == 1)
			{
				if (!mat->visite[x][y])
				{
					_PrintSuffixeMatrice(mat, x, 0);
				}
			}
		}
	}
}

void _PrintSuffixeMatrice(MatriceAdjacence* mat, int racine, int prof)
{
	int trouver = 0;
	for (int i = 0; i < mat->taille; i++)
	{
		if (mat->matrice[racine][i])
		{
			if (!mat->visite[racine][i])
			{
				mat->visite[racine][i] = 1;
				_PrintSuffixeMatrice(mat, i, prof+1);
				trouver = 1;
			}
		}
	}
	for (int j = 0; j < prof; j++)
		printf(" ");
	printf("%d\n", racine);
}

int* Connexite(MatriceAdjacence* mat)
{
	int* comp = (int*)malloc(sizeof(int) * mat->taille);

	if (comp)
	{
		for (int y = 0; y < mat->taille; y++)
		{
			int trouver = 1;
			for (int x = 0; x < mat->taille; x++)
			{
				if (x != y)
				{
					if (!mat->matrice[x][y])
						trouver = 0;
				}
			}
			if (trouver)
				comp[y] = 1;
		}
	}

	return comp;
}