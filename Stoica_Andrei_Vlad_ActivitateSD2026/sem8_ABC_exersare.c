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


struct NodArbore
{
	Masina info;
	struct NodArbore* left;
	struct NodArbore* right;
};

typedef struct NodArbore NodA;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret= atof(strtok(NULL, sep));
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


void adaugaMasinaInArbore(NodA** root, Masina masinaNoua)
{
	if ((*root) != NULL)
	{
		if (masinaNoua.id < (*root)->info.id)
		{
			adaugaMasinaInArbore(&(*root)->left, masinaNoua);
		}
		else
		{
			adaugaMasinaInArbore(&(*root)->right, masinaNoua);
		}
	}
	else
	{
		NodA* newNod = malloc(sizeof(NodA));
		
		newNod->info = masinaNoua;
		newNod->left = NULL;
		newNod->right = NULL;

		(*root) = newNod;
	}
}

NodA* citireArboreDeMasiniDinFisier(const char* numefisier)
{
	FILE* f = fopen(numefisier, "r");
	NodA* root = NULL;
	if (!f)
	{
		printf("Eroare la deschiderea fisierului");
		return root;
	}
	while (!feof(f))
	{
		adaugaMasinaInArbore(&root, citireMasinaDinFisier(f));
	}
	fclose(f);
	return root;
}

void afisarePreOrdine(NodA* root)
{
	if (root != NULL)
	{
		afisareMasina(root->info);
		afisarePreOrdine(root->left);
		afisarePreOrdine(root->right);
	}
}

void afisareInOrdine(NodA* root)
{
	if (root != NULL)
	{
		afisareInOrdine(root->left);
		afisareMasina(root->info);
		afisareInOrdine(root->right);
	}
}

void afisarePostOrdine(NodA* root)
{
	if (root != NULL)
	{
		afisarePostOrdine(root->left);
		afisarePostOrdine(root->right);
		afisareMasina(root->info);
	}
}

void dezalocareArbore(NodA** root)
{
	if ((*root) != NULL)
	{
		dezalocareArbore(&(*root)->left);
		dezalocareArbore(&(*root)->right);

		free((*root)->info.model);
		free((*root)->info.numeSofer);

		free((*root));
		(*root) = NULL;
	}
}

Masina getMasinaById(NodA* root, int id)
{
	Masina m ;
	m.id = -1;
	m.numeSofer = NULL;
	m.model = NULL;
	if (root == NULL)
	{
		return m;
	}
	else
	{
		if (id < root->info.id)
		{
			return getMasinaById(root->left, id);
		}
		else if (id > root->info.id)
		{
			return getMasinaById(root->right, id);
		}
		else
		{
			m = root->info;

			m.model = malloc(strlen(root->info.model) + 1);
			strcpy_s(m.model, strlen(root->info.model) + 1, root->info.model);

			m.numeSofer = malloc(strlen(root->info.numeSofer) + 1);
			strcpy_s(m.numeSofer, strlen(root->info.numeSofer) + 1, root->info.numeSofer);

			return m;
		}
	}
}

int determinaNumarNoduri(NodA* root) {
	if (root == NULL)
	{
		return 0;
	}
	else
	{
		return 1 + determinaNumarNoduri(root->left) + determinaNumarNoduri(root->right);
	}
}

int calculeazaInaltimeArbore(NodA* root) {
	if (root == NULL)
	{
		return 0;
	}
	else
	{
		return 1 + max(calculeazaInaltimeArbore(root->left), calculeazaInaltimeArbore(root->right));
	}
}

float calculeazaPretTotal(NodA* root) {
	if (root == NULL)
	{
		return 0;
	}
	else
	{
		return root->info.pret + calculeazaPretTotal(root->left) + calculeazaPretTotal(root->right);
	}
}

float calculeazaPretulMasinilorUnuiSofer(NodA* root,  const char* numeSofer) {

	if (root == NULL)
	{
		return 0;
	}
	else
	{
		if (strcmp(root->info.numeSofer, numeSofer) == 0)
		{
			return root->info.pret + calculeazaPretulMasinilorUnuiSofer(root->left, numeSofer) + 
			calculeazaPretulMasinilorUnuiSofer(root->right, numeSofer);
		}
		else
		{
			return calculeazaPretulMasinilorUnuiSofer(root->left, numeSofer) +
			calculeazaPretulMasinilorUnuiSofer(root->right, numeSofer);
		}
	}
}

int main() {
	FILE* f = fopen("masini_arbore.txt", "r");
	Masina m1 = citireMasinaDinFisier(f);
	afisareMasina(m1);
	printf("-----------------------------------------\n");
	NodA* root  = citireArboreDeMasiniDinFisier("masini_arbore.txt");
	afisareInOrdine(root);
	printf("-----------------------------------------\n");
	afisarePostOrdine(root);
	printf("-----------------------------------------\n");
	afisarePreOrdine(root);
	printf("-----------------------------------------\n");
	Masina m2 = getMasinaById(root, 7);
	afisareMasina(m2);
	printf("-----------------------------------------\n");
	/*Masina m3 = getMasinaById(root, 11);
	afisareMasina(m3);
	printf("-----------------------------------------\n");*/
	int nrNoduri = determinaNumarNoduri(root);
	printf("Nr noduri: %d\n", nrNoduri);
	printf("-----------------------------------------\n");
	int inaltime = calculeazaInaltimeArbore(root);
	printf("Inaltime: %d\n", inaltime);
	printf("-----------------------------------------\n");
	float pretTotal = calculeazaPretTotal(root);
	printf("Pret total: %2.f\n", pretTotal);
	float pretulMasinilorUnuiSofer = calculeazaPretulMasinilorUnuiSofer(root, "Robica");
	printf("Pret masini sofer dat: %2.f\n", pretulMasinilorUnuiSofer);
	return 0;
}