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


struct Nod {
	Student info;
	struct Nod* next;
	struct Nod* prev;
};
typedef struct Nod Nod;
struct ListaDubla
{
	Nod* first;
	Nod* last;
};
typedef struct ListaDubla ListaDubla;
Student citireStudentDinFisier(FILE* file)
{
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer,100,file);
	char* aux;
	Student s;
	s.id = atoi(strtok(buffer, sep));
	aux = strtok(NULL, sep);
	s.nume = malloc((strlen(aux)+1)*sizeof(char));
	strcpy_s(s.nume, strlen(aux)+1, aux);
	s.medie = atof(strtok(NULL,sep));
	s.serie = *strtok(NULL,sep);
	return s;
}

void afisareStudent(Student s)
{
	printf("Id: %d\n",s.id);
	printf("Nume: %s\n",s.nume);
	printf("Medie: %.2f\n",s.medie);
	printf("Serie: %c\n\n",s.serie);
}

void afisareListaStudentiDeLaInceput(ListaDubla ld) {
	Nod* primul = ld.first;
	while (primul)
	{
		afisareStudent(primul->info);
		primul = primul->next;
	}
}
void afisareListaStudentiDeLaFinal(ListaDubla ld)
{
	Nod* ultimul = ld.last;
	while (ultimul)
	{
		afisareStudent(ultimul->info);
		ultimul = ultimul->prev;
	}
}
void adaugaStudentInLista(ListaDubla* ld, Student studentNou)
{
	Nod* nou = malloc(sizeof(Nod));
	nou->info = studentNou;
	nou->next = NULL;
	nou->prev = ld->last;
	if (ld->last)
	{
		ld->last->next = nou;
	}
	else
	{
		ld->first = nou;
	}
	ld->last = nou;
}

void adaugaLaInceputInLista(ListaDubla* ld, Student studentNou) {
	Nod* nou = malloc(sizeof(Nod));
	nou->info = studentNou;
	nou->prev = NULL;
	nou->next = ld->first;
	if (ld->first)
	{
		ld->first->prev = nou;
	}
	else
	{
		ld->last = nou;
	}
	ld->first = nou;
}

ListaDubla citireListaStudentiDinFisier(const char* numefisier) {
	FILE* f = fopen(numefisier, "r");
	ListaDubla ld;
	ld.first = NULL;
	ld.last = NULL;
	while (!feof(f))
	{
		adaugaStudentInLista(&ld, citireStudentDinFisier(f));
	}
	fclose(f);
	return ld;
}

void dezalocareListaStudenti(ListaDubla* ld)
{
	Nod* cap = ld->first;
	while (cap)
	{
		Nod* urmatorul = cap->next;
		free(cap->info.nume);
		free(cap);
		cap = urmatorul;
	}
	ld->first = NULL;
	ld->last = NULL;
}

float calculeazaNotaMedieStudenti(ListaDubla ld) {
	//calculeaza pretul mediu al masinilor din lista.
	int nrStud=0;
	float sum_med=0;
	Nod* cap = ld.first;
	while (cap)
	{
		nrStud++;
		sum_med = sum_med+cap->info.medie;
		cap = cap->next;
	}
	if (nrStud == 0)
	{
		return 0;
	}
	else
	{
		return sum_med/nrStud;
	}
}

void stergeStudentDupaId(ListaDubla* ld, int id)
{
	while (ld->first && ld->first->info.id == id)
	{
		Nod* cap = ld->first;
		ld->first = ld->first->next;
		free(cap->info.nume);
		free(cap);
	}
	if (ld->first)
	{
		ld->first->prev = NULL;
	}
	else
	{
		ld->last = NULL;
	}
	while (ld->last && ld->last->info.id == id)
	{
		Nod* coada = ld->last;
		ld->last = ld->last->prev;
		free(coada->info.nume);
		free(coada);
	}
	if (ld->last)
	{
		ld->last->next = NULL;
	}
	else
	{
		ld->first = NULL;
	}
	Nod* curent = ld->first;
	while (curent)
	{
		if (curent->info.id == id)
		{
			Nod* p = curent;
			curent->prev->next = curent->next;
			curent->next->prev = curent->prev;
			curent = curent->next;
			free(p->info.nume);
			free(p);
		}
		else
		{
			curent = curent->next;
		}
	}
}
char* getNumeStudentMediemax(ListaDubla ld) {
	
	if (ld.first)
	{
		char* nume = malloc((strlen(ld.first->info.nume) + 1) * sizeof(char));
		strcpy_s(nume, strlen(ld.first->info.nume) + 1, ld.first->info.nume);
		float max = ld.first->info.medie;
		Nod* temp = ld.first->next;
		while (temp)
		{
			if (temp->info.medie > max)
			{
				max = temp->info.medie;
				free(nume);
				nume = malloc((strlen(temp->info.nume) + 1) * sizeof(char));
				strcpy_s(nume, strlen(temp->info.nume) + 1, temp->info.nume);
			}
			temp = temp->next;
		}
		return nume;
	}
	else
	{
		return NULL;
	}
}
Student creareStudent(int id, const char* nume, float medie, unsigned char serie)
{
	Student stud;
	stud.id = id;
	stud.nume = malloc((strlen(nume)+1)*sizeof(char));
	strcpy_s(stud.nume, strlen(nume) +1, nume);
	stud.medie = medie;
	stud.serie = serie;
	return stud;
}
int main() {
	FILE* f = fopen("studenti.txt", "r");
	Student s = citireStudentDinFisier(f);
	afisareStudent(s);
	ListaDubla listaStud = citireListaStudentiDinFisier("studenti.txt");
	afisareListaStudentiDeLaInceput(listaStud);
	afisareListaStudentiDeLaFinal(listaStud);
	Student newStud = creareStudent(11, "Mircea", 8.34, 'C');
	adaugaStudentInLista(&listaStud, newStud);
	printf("\n");
	afisareListaStudentiDeLaInceput(listaStud);
	printf("\n");
	printf("\n");
	Student newStud1 = creareStudent(0, "Gigica", 9.20, 'B');
	adaugaLaInceputInLista(&listaStud, newStud1);
	afisareListaStudentiDeLaInceput(listaStud);
	float medie = calculeazaNotaMedieStudenti(listaStud);
	printf("Media studentilor: %.2f\n", medie);
	stergeStudentDupaId(&listaStud, 5);
	printf("\n");
	printf("\n");
	afisareListaStudentiDeLaInceput(listaStud);
	char* nume = getNumeStudentMediemax(listaStud);
	printf("Studentul cu cea mai mare medie este: %s\n", nume);
	dezalocareListaStudenti(&listaStud);
	printf(".\n");
	afisareListaStudentiDeLaFinal(listaStud);
	afisareListaStudentiDeLaInceput(listaStud);
	printf(".\n");
	return 0;
}