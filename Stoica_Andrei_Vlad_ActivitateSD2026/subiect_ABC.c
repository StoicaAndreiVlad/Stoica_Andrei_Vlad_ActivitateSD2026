#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Pilot {
	int id;
	float greutate;
	char* echipa;
	int varsta; //ne ar interesa si varsta pilotului pentru eventuale functii(getPilotByVarsta , getVarstaMedie)
};

typedef struct Pilot Pilot;

struct NodArbore {
	Pilot info;
	struct NodArbore* left;
	struct NodArbore* right;
};
typedef struct NodArbore NodA;
void InserareNodInArbore(NodA** root, Pilot p1)
{
	if (*root)
	{
		if (p1.id < (*root)->info.id)
		{
			InserareNodInArbore(&(*root)->left, p1);
		}
		else
		{
			InserareNodInArbore(&(*root)->right, p1);
		}
	}
	else
	{
		NodA* newNod = malloc(sizeof(NodA));
		newNod->info = p1;
		newNod->left = NULL;
		newNod->right = NULL;

		(*root) = newNod;
	}
}
Pilot citestePilotDinFisier(FILE* f)
{
	char buffer[100];
	char sep[3] = ",\n";
	char* aux;
	
	fgets(buffer, 100 , f);
	Pilot p1;

	p1.id = atoi(strtok(buffer, sep));
	p1.greutate = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	p1.echipa = malloc(strlen(aux) + 1);
	strcpy_s(p1.echipa, strlen(aux) + 1, aux);

	p1.varsta = atoi(strtok(NULL, sep));

	return p1;
}

void afisarePilot(Pilot p)
{
	printf("Id pilot: %d,\n", p.id);
	printf("Greutate pilot: %.2f,\n", p.greutate);
	printf("Echipa din care face parte: %s,\n", p.echipa);
	printf("Varsta pilot: %d\n\n", p.varsta);
}

NodA* citireArboreDinFisier(const char* numefisier)
{
	FILE* f = fopen(numefisier, "r");
	NodA* arbore = NULL;
	if (!f)
	{
		printf("Eroare la deschiderea fisierului");
		return arbore;
	}
	while (!feof(f))
	{
		Pilot p = citestePilotDinFisier(f);
		InserareNodInArbore(&arbore, p);
	}
	fclose(f);
	return arbore;
}
void afisareInordine(NodA* arbore)
{
	if (arbore)
	{
		afisareInordine(arbore->left);
		afisarePilot(arbore->info);
		afisareInordine(arbore->right);
	}
}
int calculeazaNrPilotiDeLaEchipa(NodA* arbore, const char* echipa)
{
	if (!arbore)
	{
		return 0;
	}
	else
	{
		if (strcmp(arbore->info.echipa, echipa) == 0)
		{
			return 1 + calculeazaNrPilotiDeLaEchipa(arbore->left, echipa) + 
			calculeazaNrPilotiDeLaEchipa(arbore->right, echipa);
		}
		else
		{
			return calculeazaNrPilotiDeLaEchipa(arbore->left, echipa) +
				calculeazaNrPilotiDeLaEchipa(arbore->right, echipa);
		}
	}
}
Pilot getPilotById(NodA* arbore, int id)
{
	Pilot p;
	p.id = -1;
	if (!arbore)
	{
		printf("Nu exista pilot cu id-ul dat");
		return p;
	}
	else
	{
		if (id < arbore->info.id)
		{
			return getPilotById(arbore->left, id);
		}
		else if (id > arbore->info.id)
		{
			return getPilotById(arbore->right, id);
		}
		else
		{
			p = arbore->info;
			p.echipa = malloc(strlen(arbore->info.echipa)+1);
			strcpy_s(p.echipa, strlen(arbore->info.echipa) + 1, arbore->info.echipa);
			return p;
		}
	}
}
int calculeazaNrNoduri(NodA* arbore)
{
	if (!arbore)
	{
		return 0;
	}
	else
	{
		return 1 + calculeazaNrNoduri(arbore->left) +calculeazaNrNoduri(arbore->right);
	}
}
int varstaPiloti(NodA* arbore)
{
	if (!arbore)
	{
		return 0;
	}
	else
	{
		return arbore->info.varsta + varstaPiloti(arbore->left) + varstaPiloti(arbore->right);
	}
}
float varstaMediePiloti(NodA* arbore)
{
	if (!arbore)
	{
		return 0;
	}
	else
	{
		int nr = calculeazaNrNoduri(arbore);
		int varsta = varstaPiloti(arbore);
		return (float)varsta/nr;
	}
}
void dezalocareArbore(NodA** arbore)
{
	if (*arbore)
	{
		dezalocareArbore(&(*arbore)->left);
		dezalocareArbore(&(*arbore)->right);

		free((*arbore)->info.echipa);
		free((*arbore));
		(*arbore) = NULL;
	}
}
int main()
{
	NodA* arbore = citireArboreDinFisier("piloti.txt");
	afisareInordine(arbore);
	printf("--------------------------------\n");
	int nr = calculeazaNrPilotiDeLaEchipa(arbore, "RedBull");
	printf("De la echipa data sunt %d piloti\n", nr);
	printf("--------------------------------\n");
	afisarePilot(getPilotById(arbore, 4));
	printf("--------------------------------\n");
	float vmedie = varstaMediePiloti(arbore);
	printf("Varsta medie piloti: %.2f\n", vmedie);
	printf("--------------------------------\n");
	dezalocareArbore(&arbore);
	afisareInordine(arbore);
	printf("--------------------------------\n");
}