#include <stdio.h>

#include "graphe.h"

int main()
{
    MatriceAdjacence* mat = GetMatriceFromFile("graphe.txt");
    if (mat)
    {
        printf("%d\n", mat->taille);
        PrintMatriceAdjacence(mat);
        printf("nombre de chemin de longueur 2 : %d\n", NumberWayWithLength(mat, 2));

        printf("\n\nListe des successeurs pour chaque sommet :\n");
        ListeSuccesseurs* liste;
        for (int i = 0; i < mat->taille; i++)
        {
            liste = GetElementSuccessors(mat, i);
            PrintListeSuccesseur(liste);
            freeListeSuccesseur(liste);
        }

        CreateMatriceWithUserInput();

        printf("\n\n\n");

        PrintSuffixeMatrice(mat);


        printf("\n\n\n");

        int* connexe = Connexite(mat);
        for (int i = 0; i < mat->taille; i++)
        {
            if (connexe[i])
                printf("%d est fortement connexe\n", i);
            else
                printf("%d ne l'est pas\n", i);
        }

        freeMatriceAdjacence(mat);
    }

    return 0;
}