#include<stdio.h>
#include<stdlib.h>

struct Student {
	int id;
	int grupa;
	char* nume;
	float medie;
	char initiala;
};

struct Student initializare(int id, int grupa, char* nume, float medie, char initiala) {
	struct Student s;
	s.id = id;
	s.grupa = grupa;
	s.nume = (char*)malloc(sizeof(char) * (strlen(nume) + 1));
	s.medie = medie;
	strcpy(s.nume, nume);
	s.initiala = initiala;
	return s;
}

void afisare(struct Student s) {
	printf("%d. %s(%c) din grupa %d are media %5.2f", s.id, s.nume, s.initiala, s.grupa, s.medie);
}

void modifica_Nume(struct Student* s, char* noulNume) {
	if (strlen(noulNume) > 2)
	{
		free(s->nume);
		(*s).nume = (char*)malloc(sizeof(char) * strlen(noulNume) + 1); //acelasi lucru
		strcpy(s->nume, noulNume);
	}
}

void dezalocare(struct Student* s) {
	free(s->nume);
	s->nume = NULL;
}

char* comparaStudenti(struct Student student1, struct Student student2)
{
	if (student1.medie > student2.medie) return student1.nume;
	else return student2.nume;
}
int main() {
	struct Student s;
	s = initializare(1, 1059, "Popescu", 8.3, 'G');
	struct Student s2 = initializare(2, 1059, "Vasilescu Gigel", 8.3, 'A');
	printf("%s", comparaStudenti(s, s2));
	printf("\n");
	afisare(s);
	printf("\n");
	modifica_Nume(&s, "Ionescu Gigel");
	afisare(s);
	printf("\n");
	dezalocare(&s);

	return 0;
}