#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct Student {
	int id;
	char* nume;
	float medie;
	unsigned char serie;
};
typedef struct Student Student;

typedef struct Nod
{
	Student info;
	struct Nod* next;
};
typedef struct Nod Nod;


struct HashTable {
	int dim;
	Nod** vector;
};
typedef struct HashTable HashTable;

Student citireStudentDinFisier(FILE* file) {
	char buffer[100];
	fgets(buffer, 100, file);
	char sep[3] = ",\n";
	Student stud;
	stud.id = atoi(strtok(buffer, sep));
	char* aux;
	aux = strtok(NULL, sep);
	stud.nume = malloc((strlen(aux) + 1)*sizeof(char));
	strcpy_s(stud.nume, strlen(aux) + 1, aux);
	stud.medie = atof(strtok(NULL, sep));
	stud.serie = *strtok(NULL, sep);
	return stud;
}

void afisareStudent(Student stud) {
	printf("Id: %d\n", stud.id);
	printf("Nume: %s\n", stud.nume);
	printf("Medie: %.2f\n", stud.medie);
	printf("Serie: %c\n\n", stud.serie);
}

void afisareListaStudenti(Nod* lista) {
	Nod* cap = lista;
	while (cap)
	{
		afisareStudent(cap->info);
		cap = cap->next;
	}
}

void adaugaStudentInLista(Nod** lista, Student newStud) {
	Nod* temp = malloc(sizeof(Nod));
	temp->info.id = newStud.id;
	temp->info.nume = malloc((strlen(newStud.nume)+1)*sizeof(char));
	strcpy_s(temp->info.nume, strlen(newStud.nume) + 1, newStud.nume);
	temp->info.medie = newStud.medie;
	temp->info.serie = newStud.serie;
	temp->next = NULL;
	if (*lista)
	{
		Nod* cap = (*lista);
		while (cap->next)
		{
			cap = cap->next;
		}
		cap->next = temp;
	}
	else
	{
		(*lista) = temp;
	}
}


HashTable initializareHashTable(int dimensiune) {
	HashTable ht;
	ht.dim = dimensiune;
	ht.vector = malloc(sizeof(Nod*)*dimensiune);
	for (int i = 0; i < dimensiune; i++)
	{
		ht.vector[i] = NULL;
	}
	return ht;
}

int calculeazaHash(int id, int dimensiune) {
	return (id*3)%dimensiune;
}

void inserareStudentInTabela(HashTable hash, Student newStud) {
	int index = calculeazaHash(newStud.id, hash.dim);
	adaugaStudentInLista(&hash.vector[index], newStud);
}

HashTable citireStudentiDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	HashTable ht = initializareHashTable(7);
	if (f == NULL)
	{
		return ht;
	}
	else
	{
		while (!feof(f))
		{
			inserareStudentInTabela(ht, citireStudentDinFisier(f));
		}
	}
	fclose(f);
	return ht;
}

void afisareTabelaDeStudenti(HashTable ht) {
	for (int i = 0; i < ht.dim; i++)
	{
		printf("Clusterul cu numarul %d\n",i+1);
		afisareListaStudenti(ht.vector[i]);
		printf("---------------------------\n");
	}
}

void dezalocareTabelaDeStudenti(HashTable *ht) {
	for (int i = 0; i < ht->dim; i++)
	{
		Nod* cap = ht->vector[i];
		while (cap)
		{
			Nod* temp = cap;
			cap = cap->next;
			free(temp->info.nume);
			free(temp);
		}
		ht->vector[i] = NULL;
	}
	free(ht->vector);
	ht->vector = NULL;
	ht->dim = 0;
}

float* calculeazaMediiPerClustere(HashTable ht, int* nrClustere) {

	*nrClustere = 0;
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.vector[i])
		{
			(*nrClustere)++;
		}
	}
	if ((*nrClustere))
	{
		float* vectMedii = malloc((*nrClustere) * sizeof(float));
		int index = 0;
		for (int i = 0; i < ht.dim; i++)
		{
			if (ht.vector[i])
			{
				int nrNoduri = 0;
				float sum_med = 0;
				Nod* cap = ht.vector[i];
				while (cap)
				{
					nrNoduri++;
					sum_med += cap->info.medie;
					cap = cap->next;
				}
				vectMedii[index] = sum_med / nrNoduri;
				index++;
			}
		}
		return vectMedii;
	}
	else
	{
		return NULL;
	}
		
}

Student getStudentDupaId(HashTable ht, int id) {
	Student s;
	s.id = -1;
	s.nume = NULL;
	s.medie = 0;
	s.serie = 0;
	int cheie = calculeazaHash(id, ht.dim);
	Nod* cap = ht.vector[cheie];
	while (cap)
	{
		if (cap->info.id == id)
		{
			s.id = cap->info.id;
			s.nume = malloc((strlen(cap->info.nume)+1)*sizeof(char));
			strcpy_s(s.nume, strlen(cap->info.nume) + 1, cap->info.nume);
			s.medie = cap->info.medie;
			s.serie = cap->info.serie;
			return s;
		}
		cap = cap->next;	
	}
	return s;
}

int main() {
	FILE* f = fopen("studenti.txt", "r");
	Student s1 = citireStudentDinFisier(f);
	afisareStudent(s1);
	HashTable ht = initializareHashTable(7);
	ht = citireStudentiDinFisier("studenti.txt");
	afisareTabelaDeStudenti(ht);
	printf("-----------------------------------------\n\n");
	int id = 2;
	char* nume = "Florin Prunea";
	float medie = 6.7;
	char serie = 'A';
	Student s;
	s.id = id;
	s.nume = malloc((strlen(nume)+1)*sizeof(char));
	strcpy_s(s.nume, strlen(nume) + 1, nume);
	s.medie = medie;
	s.serie = serie;
	inserareStudentInTabela(ht, s);
	afisareTabelaDeStudenti(ht);
	printf("-----------------------------------------\n\n");
	Student newStud;
	int id1 = 8;
	newStud = getStudentDupaId(ht, id1);
	afisareStudent(newStud);
	printf("-----------------------------------------\n\n");
	int nrClustere = 0;
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.vector[i])
		{
			printf("Media pe clusterul %d este: %.2f\n",i+1,calculeazaMediiPerClustere(ht,&nrClustere)[i]);
		}
		else
		{
			printf("Nu se afla noduri in acest cluster");
		}
	}
	dezalocareTabelaDeStudenti(&ht);
	printf("Tabela de studenti dupa dezalocare: \n");
	afisareTabelaDeStudenti(ht);
	printf("--------------------------------------\n");
	return 0;
}