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

Student citireStudentDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	Student s;
	fgets(buffer, 100, file);
	s.id = atoi(strtok(buffer, sep));
	char* aux = strtok(NULL, sep);
	s.nume = malloc((strlen(aux) + 1) * sizeof(char));
	strcpy_s(s.nume, strlen(aux) + 1, aux);
	s.medie = atof(strtok(NULL, sep));
	s.serie = *strtok(NULL, sep);
	return s;
}

void afisareStudent(Student s)
{
	printf("Id: %d\n", s.id);
	printf("Nume: %s\n", s.nume);
	printf("Medie: %.2f\n", s.medie);
	printf("Serie: %c\n\n", s.serie);
}
struct Nod {
	Student info;
	struct Nod* next;
};
typedef struct Nod Nod;
void pushStack(Nod** stiva, Student newStud) {
	Nod* first = malloc(sizeof(Nod));
	first->info = newStud;
	first->next = (*stiva);
	(*stiva) = first;
}

Student popStack(Nod** stiva) {
	Student newStud;
	if (*stiva)
	{
		Nod* first = (*stiva);
		newStud.id = first->info.id;
		newStud.nume = malloc((strlen(first->info.nume) + 1) * sizeof(char));
		strcpy_s(newStud.nume, strlen(first->info.nume) + 1, first->info.nume);
		newStud.medie = first->info.medie;
		newStud.serie = first->info.serie;
		free(first->info.nume);
		(*stiva) = (*stiva)->next;
		free(first);

	}
	else
	{
		newStud.id = -1;
		newStud.nume = NULL;
		newStud.medie = 0;
		newStud.serie = 0;
	}
	return newStud;
}

int emptyStack(Nod* Stiva) {
	return Stiva == NULL;
}

Nod* citireStackStudentiDinFisier(const char* numeFisier) {

	FILE* f = fopen(numeFisier, "r");
	if (f == NULL)
	{
		return NULL;
	}
	Nod* stack = NULL;
	while (!feof(f))
	{
		pushStack(&stack, citireStudentDinFisier(f));
	}
	fclose(f);
	return stack;

}

void dezalocareStivaDeStudenti(Nod** stiva) {
	while (*stiva)
	{
		Nod* first = *stiva;
		*stiva = (*stiva)->next;
		free(first->info.nume);
		free(first);
	}
}

int size(Nod* stack) {
	int nrElemStiva = 0;
	while (stack)
	{
		nrElemStiva++;
		stack = stack->next;
	}
	return nrElemStiva;
}

//QUEUE
//Alegeti prin ce veti reprezenta coada si creati structura necesara acestei cozi
//putem reprezenta o coada prin LSI, LDI sau vector
struct NodCoada
{
	Student info;
	struct NodCoada* next;
	struct NodCoada* prev;
};
typedef struct NodCoada NodC;
struct Coada
{
	NodC* first;
	NodC* last;
};
typedef struct Coada Coada;
void enqueue(Coada* c, Student stud) {
	NodC* temp = malloc(sizeof(NodC));
	temp->info = stud;
	temp->next = NULL;
	temp->prev = c->last;
	if (c->last)
	{
		c->last->next = temp;
	}
	else
	{
		c->first = temp;
	}
	c->last = temp;
}
Student dequeue(Coada* c) {
	Student rezultat;
	if (c->first)
	{
		NodC* cap = c->first;
		rezultat.id = cap->info.id;
		rezultat.nume = malloc((strlen(cap->info.nume) + 1) * sizeof(char));
		strcpy_s(rezultat.nume, strlen(cap->info.nume) + 1, cap->info.nume);
		rezultat.medie = cap->info.medie;
		rezultat.serie = cap->info.serie;
		c->first = cap->next;
		if (c->first)
		{
			c->first->prev = NULL;
		}
		else
		{
			c->last = NULL;
		}
		free(cap->info.nume);
		free(cap);
	}
	else
	{
		rezultat.id = -1;
		rezultat.nume = NULL;
		rezultat.medie = 0;
		rezultat.serie = 0;
	}
	return rezultat;
}

Coada citireCoadaDeStudentiDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Coada c;
	c.first = NULL;
	c.last = NULL;
	while (!feof(f))
	{
		enqueue(&c, citireStudentDinFisier(f));
	}
	fclose(f);
	return c;
}

void dezalocareCoadaDeStudenti(Coada* c) {
	while (c->first)
	{
		NodC* cap = c->first;
		c->first = cap->next;
		if (c->first)
		{
			c->first->prev = NULL;
		}
		else
		{
			c->last = NULL;
		}
		free(cap->info.nume);
		free(cap);
	}
}
Student getStudentByID(Nod* stiva, int id)
{
	Nod* cap = stiva;
	while (cap)
	{
		if (cap->info.id == id)
		{
			return cap->info;
		}
		else
		{
			cap = cap->next;
		}
	}
	Student rezultat;
	rezultat.id = -1;
	rezultat.nume = NULL;
	rezultat.medie = 0;
	rezultat.serie = 0;
	return rezultat;
}
Student extractStudentById(Nod** stiva, int id)
{
	Student rezultat;
	rezultat.id = -1;
	rezultat.nume = NULL;
	rezultat.medie = 0;
	rezultat.serie = 0;
	Nod* stivaNoua = NULL;
	while (*stiva)
	{
		Student studentNou = popStack(stiva);
		if (studentNou.id == id)
		{
			rezultat = studentNou;
			break;
		}
		pushStack(&stivaNoua, studentNou);
	}
	while (stivaNoua)
	{
		pushStack(stiva, popStack(&stivaNoua));
	}
	return rezultat;
}
float calculeazamedie(Nod* stiva)
{
	float sum_med = 0;
	int nrStud = 0;
	Nod* cap = stiva;
	while (cap)
	{
		nrStud++;
		sum_med += cap->info.medie;
		cap = cap->next;
	}
	if (nrStud == 0)
	{
		return 0;
	}
	else
	{
		return sum_med / nrStud;
	}
}
int isEmptyQueue(Coada C)
{
	return (C.first == NULL);
}
int main() {
	FILE* f = fopen("studenti.txt", "r");
	Student stud = citireStudentDinFisier(f);
	afisareStudent(stud);
	Nod* stiva = citireStackStudentiDinFisier("studenti.txt");
	printf("Se afla %d noduri in stiva \n", size(stiva));
	Student newstud = popStack(&stiva);
	afisareStudent(newstud);
	printf("Se afla %d noduri in stiva \n", size(stiva));
	pushStack(&stiva, newstud);
	printf("Se afla %d noduri in stiva \n\n", size(stiva));
	
	
	printf("Studentul cu ID-ul 4: ");
	afisareStudent(extractStudentById(&stiva, 4));
	printf("Se afla %d noduri in stiva \n", size(stiva));
	printf("Media studentilor este: %.2f\n",calculeazamedie(stiva));
	dezalocareStivaDeStudenti(&stiva);
	printf("Se afla %d noduri in stiva \n", size(stiva));
	if (emptyStack(stiva))
	{
		printf("Stiva este goala\n");
	}
	else
	{
		printf("Stiva nu este goala\n");
	}
	printf("\n");

	Coada coada = citireCoadaDeStudentiDinFisier("studenti.txt");
	if (isEmptyQueue(coada))
	{
		printf("Coada este goala\n");
	}
	else
	{
		printf("Coada nu este goala\n");
	}
	printf("Primul student de la coada este: ");
	afisareStudent(dequeue(&coada));
	dezalocareCoadaDeStudenti(&coada);
	if (isEmptyQueue(coada))
	{
		printf("Coada este goala\n");
	}
	else
	{
		printf("Coada nu este goala\n");
	}
	return 0;
}