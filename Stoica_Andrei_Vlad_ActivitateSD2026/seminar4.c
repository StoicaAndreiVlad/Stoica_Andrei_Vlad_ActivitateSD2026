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

struct Nod {
	Masina info;
	struct Nod* next;
};
typedef struct Nod Nod;

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

void afisareListaMasini(Nod* cap) {
	while (cap) {
		afisareMasina(cap->info);
		cap = cap->next;
	}
}

void adaugaMasinaInLista(Nod** cap, Masina masinaNoua) {
	Nod* temp = malloc(sizeof(Nod));
	temp->info = masinaNoua;
	temp->next = NULL;
	if (!(*cap)) {
		*cap = temp;
	}
	else {
		Nod* p = *cap;
		while (p->next) {
			p = p->next;
		}
		p->next = temp;
	}
}

void adaugaLaInceputInLista(Nod** cap, Masina masinaNoua) {
	Nod* temp = malloc(sizeof(Nod));
	temp->info = masinaNoua;
	temp->next = *cap;
	*cap = temp;

}

void* citireListaMasiniDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	Nod* cap = NULL;
	while (!feof(file)) {
		adaugaMasinaInLista(&cap, citireMasinaDinFisier(file));
	}
	fclose(file);
	return cap;
}

void dezalocareListaMasini(Nod** temp) {
	while ((*temp))
	{
		(*temp)->info.model = NULL;
		(*temp)->info.numeSofer = NULL;
		Nod* nou = *temp;
		nou = nou->next;
		free(*temp);
		(*temp) = nou;
	}

}

float calculeazaPretMediu(Nod* cap) {
	float suma = 0;
	int nrMasini = 0;
	while (cap) {
		suma += cap->info.pret;
		nrMasini++;
		cap = cap->next;
	}
	if (nrMasini > 0) {
		return suma / nrMasini;
	}
	return 0;
}

void stergeMasiniDinSeria(Nod** cap, char serieCautata) {
	//sterge toate masinile din lista care au seria primita ca parametru.
	//tratati situatia ca masina se afla si pe prima pozitie, si pe ultima pozitie
	while ((*cap) && (*cap)->info.serie == serieCautata) {
		Nod* temp = *cap;
		*cap = (*cap)->next;
		free(temp);
	}
	Nod* curent = *cap;
	Nod* before = NULL;
	while (curent != NULL)
	{
		if (curent->info.serie == serieCautata) {
			before->next = curent->next;
			free(curent);
			curent = before->next;
		}
		else {
			before = curent;
			curent = curent->next;
		}
	}

}

float calculeazaPretulMasinilorUnuiSofer(Nod* cap, const char* numeSofer) {
	float suma = 0;
	float count = 0;
	while (cap) {
		if (strcmp(cap->info.numeSofer, numeSofer) == 0) {
			suma += cap->info.pret;
			count++;
		}
		cap = cap->next;
	}
	return suma / count;
}

int main() {
	Nod* cap = citireListaMasiniDinFisier("masini.txt");
	printf("Lista citita din fisier:\n");
	afisareListaMasini(cap);
	printf("\n");

	printf("Citire lista si adaugare masina la inceput:\n");
	adaugaLaInceputInLista(&cap, (Masina) { 0, 4, 20000.0f, "Dacia", "Ion", 'A' });
	afisareListaMasini(cap);
	printf("\n");

	printf("Adaugare masina la final: \n");
	adaugaMasinaInLista(&cap, (Masina) { 60, 2, 4000.0f, "Fabia", "Alex", 'D' });
	afisareListaMasini(cap);
	printf("\n");

	printf("Pretul mediu al masinilor: %2.f \n\n", calculeazaPretMediu(cap));

	printf("Pretul mediu al masinilor detinute de %s: %2.f\n\n", (char*)"Ionescu", calculeazaPretulMasinilorUnuiSofer(cap, "Ionescu"));

	printf("Stergere din lista masina cu seria 'A':\n");
	stergeMasiniDinSeria(&cap, 'A');
	afisareListaMasini(cap);
	printf("\n");

	printf("Lista dezalocata:\n");
	dezalocareListaMasini(&cap);
	afisareListaMasini(cap);
	printf("\n");
	return 0;
	//test
}