#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}



struct NodListaPrincipala
{
	Masina info;
	struct NodListaPrincipala* next;
	struct NodListaSecundara* vecini;
};

typedef struct NodListaPrincipala NodP;

struct NodListaSecundara
{
	NodP* info;
	struct NodListaSecundara* next;
};

typedef struct NodListaSecundara NodS;



void inserareNodInListaPrincipala(NodP** graf, Masina masinaNoua)
{
	NodP* newNod = malloc(sizeof(NodP));
	newNod->info = masinaNoua;
	newNod->next = NULL;
	newNod->vecini = NULL;

	if (*graf)
	{
		NodP* aux = *graf;
		while (aux->next)
		{
			aux = aux->next;
		}
		aux->next = newNod;
	}
	else
	{
		*graf = newNod;
	}
}

void inserareNodInListaSecundara(NodS** listaVecini, NodP* newVecin)
{
	NodS* newNod = malloc(sizeof(NodS));
	newNod->info = newVecin;
	newNod->next = NULL;

	if (*listaVecini)
	{
		NodS* aux = *listaVecini;
		while (aux->next)
		{
			aux = aux->next;
		}
		aux->next = newNod;
	}
	else
	{
		*listaVecini = newNod;
	}
}


NodP* cautaNodDupaID(NodP* listaPrincipala, int id) {
	NodP* nodCautat = NULL;
	if (listaPrincipala)
	{
		nodCautat = listaPrincipala;
		while (nodCautat)
		{
			if (nodCautat->info.id == id)
			{
				return nodCautat;
			}
			else
			{
				nodCautat = nodCautat->next;
			}
		}
	}
	return nodCautat;
}


void inserareMuchie(NodP* listaPrincipala, int idStart, int idStop) {
	NodP* nodStart = cautaNodDupaID(listaPrincipala, idStart);
	NodP* nodStop = cautaNodDupaID(listaPrincipala, idStop);

	if(nodStart && nodStop)
	{
		inserareNodInListaSecundara(&nodStart->vecini, nodStop);
		inserareNodInListaSecundara(&nodStop->vecini, nodStart);
	}
	else
	{
		printf("Nu s au gasit nodurile cu id-urile date\n");
	}
}


NodP* citireNoduriMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	NodP* listaPrincipala = NULL;
	if (!f)
	{
		printf("Eroare la deschiderea fisierului");
		return listaPrincipala;
	}
	else
	{
		while (!feof(f))
		{
			Masina m = citireMasinaDinFisier(f);
			inserareNodInListaPrincipala(&listaPrincipala, m);
		}
		fclose(f);
		return listaPrincipala;
	}
}

void citireMuchiiDinFisier(NodP* graf, const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	if (!f)
	{
		printf("Eroare la deschiderea fisierului");
	}
	else
	{
		while (!feof(f))
		{
			int idStart,idStop;
			fscanf(f, "%d %d", &idStart, &idStop);
			inserareMuchie(graf, idStart, idStop);
		}
		fclose(f);
	}
}

void dezalocareNoduriGraf(NodP** listaPrincipala) {
	
	NodP* temp = *listaPrincipala;
	while (temp)
	{
		NodS* cap = temp->vecini;
		while (cap)
		{
			NodS* tempS = cap;
			cap = cap->next;
			free(tempS);
		}
		NodP* tempP = temp;
		temp = temp->next;

		free(tempP->info.model);
		free(tempP->info.numeSofer);
		free(tempP);
	}
	*listaPrincipala = NULL;
}
void afiseazaVecini(NodP* graf, int id)
{
	NodP* nodCautat = cautaNodDupaID(graf, id);
	if (nodCautat)
	{	
		printf("Vecinii lui %d sunt: ", nodCautat->info.id);
		NodS* cap = nodCautat->vecini;
		while (cap)
		{
			printf("%d ", cap->info->info.id);
			cap = cap->next;
		}
		printf("\n");
	}
	else
	{
		printf("Nodul cu id-ul dat nu exista \n");	
	}
}
int main() {
	FILE* f = fopen("masini.txt", "r");
	FILE* g = fopen("muchii.txt", "r");
	NodP* graf = citireNoduriMasiniDinFisier("masini.txt");
	citireMuchiiDinFisier(graf, "muchii.txt");
	afiseazaVecini(graf, 3);
	printf("----------------------------\n");
	dezalocareNoduriGraf(&graf);
	afiseazaVecini(graf, 3);
	printf("----------------------------\n");
	return 0;
}