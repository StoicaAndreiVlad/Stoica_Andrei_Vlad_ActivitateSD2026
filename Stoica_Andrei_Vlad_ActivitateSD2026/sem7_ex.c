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


struct Heap
{
	int lungime;
	int nrElem;
	Masina* vector;
}; 

typedef struct Heap Heap;

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

Heap initializareHeap(int lungimeHeap)
{
	Heap heap;
	heap.lungime = lungimeHeap;
	heap.nrElem = 0;
	heap.vector = malloc(sizeof(Masina)*lungimeHeap);
	return heap;
}
void filtrareHeap(Heap heap, int pozitieNod)
{
	int left = 2 * pozitieNod + 1;
	int right = 2 * pozitieNod +2;
	int pozitieMax = pozitieNod;

	if (left < heap.nrElem && heap.vector[left].pret > heap.vector[pozitieMax].pret)
	{
		pozitieMax = left;
	}
	if (right < heap.nrElem && heap.vector[right].pret > heap.vector[pozitieMax].pret)
	{
		pozitieMax = right;
	}
	if (pozitieMax != pozitieNod)
	{
		Masina aux = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = heap.vector[pozitieMax];
		heap.vector[pozitieMax] = aux;

		filtrareHeap(heap, pozitieMax);
	}
}
Heap citireHeapMasiniDinFisier(const char* numefisier)
{
	FILE* f = fopen(numefisier, "r");
	if (!f)
	{
		printf("Eroare la deschiderea fisierului");
		return initializareHeap(0);
	}
	else
	{
		Heap heap;
		heap = initializareHeap(10);
		while (!feof(f))
		{
			heap.vector[heap.nrElem++] = citireMasinaDinFisier(f);
		}
		fclose(f);
		for (int i = (heap.nrElem - 2) / 2; i >= 0; i--)
		{
			filtrareHeap(heap, i);
		}
		return heap;
	}
}
void afisareHeap(Heap heap)
{
	for (int i = 0; i < heap.nrElem; i++)
	{
		afisareMasina(heap.vector[i]);
	}
}
void afisareHeapAscuns(Heap heap)
{
	for (int i = heap.nrElem; i < heap.lungime; i++)
	{
		afisareMasina(heap.vector[i]);
	}
}
Masina extrageMasina(Heap* heap)
{
	Masina aux;
	if (heap->nrElem <= 0)
	{
		printf("Nu exista elemente active in Heap.");
		aux.id = -1;
		return aux;
	}
	else
	{
		heap->nrElem--;

		aux = heap->vector[0];
		heap->vector[0] = heap->vector[heap->nrElem];
		heap->vector[heap->nrElem] = aux;

		filtrareHeap(*heap, 0);
		return aux;
	}
}
void dezalocareHeap(Heap* heap)
{
	for (int i = 0; i < heap->lungime; i++)
	{
		free(heap->vector[i].model);
		free(heap->vector[i].numeSofer);
	}
	free(heap->vector);
	heap->vector = NULL;
	heap->lungime = 0;
	heap->nrElem = 0;
}
int main() {
	FILE* f = fopen("masini.txt", "r");
	Masina m1;
	m1 = citireMasinaDinFisier(f);
	afisareMasina(m1);
	Heap heap;
	heap = initializareHeap(10);
	heap = citireHeapMasiniDinFisier("masini.txt");
	afisareHeap(heap);
	printf("----------------------------------------\n");
	Masina m2 = extrageMasina(&heap);
	afisareMasina(m2);
	printf("----------------------------------------\n");
	afisareHeap(heap);
	printf("----------------------------------------\n");
	afisareHeapAscuns(heap);
	printf("----------------------------------------\n");
	dezalocareHeap(&heap);
	afisareHeap(heap);
	printf("----------------------------------------\n");
	return 0;
}