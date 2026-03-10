#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
// operatorul index = deplasare si dereferentiere
struct Telefon {
	int id;
	int RAM;
	char* producator;
	float pret;
	char serie;
};
struct Telefon initializare(int id, int RAM, char* producator, float pret, char serie) {
	struct Telefon t;
	t.id = id;
	t.RAM = RAM;
	t.producator = (char*)malloc((strlen(producator) + 1) * sizeof(char));
	strcpy(t.producator, producator);
	t.pret = pret;
	t.serie = serie;
	return t;
}
struct Telefon copiazaTelefon(struct Telefon t) {
	struct Telefon telefon = initializare(t.id, t.RAM, t.producator, t.pret, t.serie);
	return telefon;
}
void afisare(struct Telefon t) {
	printf("Id: %d\nRAM: %d\nProducator: %s\nPret: %5.1f\nSerie: %c\n\n", t.id, t.RAM, t.producator, t.pret, t.serie);
}
void afisareVector(struct Telefon* vector, int nrElemente) {
	// afisarea elementelor din vector apeland functia afisare
	for (int i = 0; i < nrElemente; i++)
		afisare(vector[i]);
}
struct Telefon* copiazaPrimeleNElemente(struct Telefon* vector, int nrElemente, int nrElementeCopiate) {
	//copiem intr-un vector nou pe care il vom returna primele nrElementeCopiate
	if (nrElementeCopiate > nrElemente) {
		nrElementeCopiate = nrElemente;
	}
	if (nrElementeCopiate <= nrElemente) {
		struct Telefon* vectorNou = (struct Telefon*)malloc(sizeof(struct Telefon) * nrElementeCopiate);
		for (int i = 0; i < nrElementeCopiate; i++) {
			vectorNou[i] = copiazaTelefon(vector[i]);
		}
		return vectorNou;
	}
}
void dezalocare(struct Telefon** vector, int* nrElemente) {
	for (int i = 0; i < *nrElemente; i++) {
		free((*vector)[i].producator);
		(*vector)[i].producator = NULL;
	}
	free(*vector);
	*vector = NULL;
	*nrElemente = 0;
}
void copiazaTelefoaneScumpe(struct Telefon* vector, char nrElemente, float pretMinim, struct Telefon** vectorNou, int* dimensiune) {
	// parametrul prag poate fi modificat in functie de
	// tipul atributului ales pentru a indeplini o conditie
	// este creat un nou vector cu elementele care indeplinesc acea conditie
	*dimensiune = 0;
	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].pret >= pretMinim) {
			(*dimensiune)++;
		}
	}
	*vectorNou = malloc((*dimensiune) * sizeof(struct Telefon));
	for (int i = 0, j = 0; i < nrElemente; i++) {
		if (vector[i].pret >= pretMinim) {
			(*vectorNou)[j] = copiazaTelefon(vector[i]);
			j += 1;
		}
	}
}
/*
TEMA:
struct Telefon getPrimulElementConditionat(struct Telefon* vector, int nrElemente, const char* conditie) {
//trebuie cautat elementul care indeplineste o conditie
//dupa atributul de tip char*. Acesta este returnat.
struct Telefon t;
t.id = 1;
return t;
}
*/
int main() {
	struct Telefon t;
	t = initializare(1, 8, "Samsung", 3001, 'A');
	afisare(t);
	int nrTelefoane = 3;
	int nrTelefoaneCopiate = 1;
	struct Telefon* telefoane = (struct Telefon*)malloc(sizeof(struct Telefon) * nrTelefoane);
	telefoane[0] = initializare(1, 8, "Samsung", 2000, 'A');
	telefoane[1] = initializare(2, 8, "Samsung", 3000.5, 'S');
	telefoane[2] = t;
	afisareVector(telefoane, nrTelefoane);
	struct Telefon* telefoaneCopiate = copiazaPrimeleNElemente(telefoane, nrTelefoane, nrTelefoaneCopiate);
	printf("Vector telefoane copiate: \n");
	afisareVector(telefoaneCopiate, nrTelefoaneCopiate);
	dezalocare(&telefoaneCopiate, &nrTelefoaneCopiate);
	printf("Vector telefoane copiate dupa dezalocare: \n");
	afisareVector(telefoaneCopiate, nrTelefoaneCopiate);
	struct Telefon* vectorNou;
	int dimensiune;
	copiazaTelefoaneScumpe(telefoane, nrTelefoane, 2500, &vectorNou, &dimensiune);
	printf("\nVector telefoane dupa conditie: \n");
	afisareVector(vectorNou, dimensiune);
	return 0;
}