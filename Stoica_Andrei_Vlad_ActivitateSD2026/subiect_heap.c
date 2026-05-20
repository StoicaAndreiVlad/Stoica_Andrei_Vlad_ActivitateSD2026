#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Telefon
{
	int id;
	float pret;
	char* producator;
	int sanatateBaterie; //util pt a calcula media sanatatii bateriilor
};

typedef struct Telefon Telefon;

struct Heap
{
	Telefon* vector;
	int lungime;
	int nrElem;
};
typedef struct Heap Heap;
void filtreazaHeap(Heap heap, int nodCurent)
{
	int left = 2 * nodCurent + 1;
	int right = 2* nodCurent +2;
	int pozitieMin = nodCurent;

	if (left < heap.nrElem && heap.vector[left].id < heap.vector[pozitieMin].id)
	{
		pozitieMin = left;
	}
	if (right < heap.nrElem && heap.vector[right].id < heap.vector[pozitieMin].id)
	{
		pozitieMin = right;
	}
	if (pozitieMin != nodCurent)
	{
		Telefon aux = heap.vector[nodCurent];
		heap.vector[nodCurent] = heap.vector[pozitieMin];
		heap.vector[pozitieMin] = aux;

		filtreazaHeap(heap, pozitieMin);
	}
}
Telefon citireTelefonDinFisier(FILE* f)
{
	char buffer[100];
	char sep[3] = ",\n";
	char* aux;
	fgets(buffer, 100, f);
	Telefon t;
	t.id = atoi(strtok(buffer,sep));
	t.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL,sep);
	t.producator = malloc(strlen(aux)+1);
	strcpy_s(t.producator, strlen(aux) + 1, aux);
	t.sanatateBaterie = atoi(strtok(NULL,sep));
	return t;
}
void afisareTelefon(Telefon t)
{
	printf("Id telefon: %d\n", t.id);
	printf("Pret telefon: %.2f\n", t.pret);
	printf("Producator telefon: %s\n", t.producator);
	printf("Sanatate baterie telefon: %d\n\n", t.sanatateBaterie);
}
Heap initializareHeap(int lungime)
{
	Heap heap;
	heap.lungime = lungime;
	heap.nrElem = 0;
	heap.vector = malloc(lungime*sizeof(Telefon));
	return heap;
}
Heap citireHeapTelefoaneDinFisier(const char* numefisier)
{
	FILE* f = fopen(numefisier, "r");
	if (!f)
	{	
		printf("Eroare la deschiderea fisierului");
		return initializareHeap(0);
	}
	Heap heap = initializareHeap(10);
	while (!feof(f))
	{
		
		Telefon t = citireTelefonDinFisier(f);
		heap.vector[heap.nrElem++] = t;
	}
	fclose(f);
	heap.lungime = heap.nrElem;
	for (int i = (heap.nrElem - 2) / 2; i >= 0; i--)
	{
		filtreazaHeap(heap, i);
	}
	return heap;
}

void afisareHeapElementeActive(Heap heap)
{
	for (int i = 0; i < heap.nrElem; i++)
	{
		afisareTelefon(heap.vector[i]);
	}
}
void afisareHeapElementeAscunse(Heap heap)
{
	for (int i = heap.nrElem; i < heap.lungime; i++)
	{
		afisareTelefon(heap.vector[i]);
	}
}
Telefon extrageTelefonDinHeap(Heap* heap)
{
	Telefon rezultat;
	if (heap->nrElem <= 0)
	{
		printf("Heap-ul este gol");
		rezultat.id = -1;
		rezultat.producator = NULL;
		return rezultat;
	}
	heap->nrElem--;
	rezultat = heap->vector[0];
	heap->vector[0] = heap->vector[heap->nrElem];
	heap->vector[heap->nrElem] = rezultat;

	filtreazaHeap(*heap, 0);

	return rezultat;
}
void dezalocareHeap(Heap* heap)
{
	if (heap)
	{
		for (int i = 0; i < heap->lungime; i++)
		{
			free(heap->vector[i].producator);
		}
		free(heap->vector);
		heap->vector = NULL;
		heap->nrElem = 0;
		heap->lungime = 0;
	}
	
}
int main()
{		
	Heap heap = citireHeapTelefoaneDinFisier("telefoane.txt");
	afisareHeapElementeActive(heap);
	printf("----------------------------------\n");
	afisareTelefon(extrageTelefonDinHeap(&heap));
	printf("----------------------------------\n");
	afisareHeapElementeActive(heap);
	printf("----------------------------------\n");
	afisareHeapElementeAscunse(heap);
	printf("----------------------------------\n");
	dezalocareHeap(&heap);
	printf("----------------------------------\n");
	return 0;
}