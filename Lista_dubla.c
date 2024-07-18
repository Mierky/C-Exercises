#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
	char* nume;
	int nrCarti;
	int nrCititori;
}Biblioteca;

typedef struct Nod Nod;
typedef struct ListaDubla ListaDubla;

struct Nod {
	Biblioteca info;
	Nod* next;
	Nod* prev;
};

struct ListaDubla {
	Nod* head;
	Nod* tail;
};

Biblioteca initializare(const char* nume, int carti, int cititori) {
	Biblioteca b;
	b.nrCarti = carti;
	b.nrCititori = cititori;
	b.nume = (char*)malloc(sizeof(char) * (strlen(nume) + 1));
	strcpy(b.nume, nume);
	return b;
}

void afisareBiblioteca(Biblioteca b) {
	printf("Biblioteca %s are %d carti si %d cititori.\n", b.nume, b.nrCarti, b.nrCititori);
}

void inserareInceput(ListaDubla* lista, Biblioteca b) {
	Nod* nod = (Nod*)malloc(sizeof(Nod));
	nod->info = b;
	nod->next = (*lista).head;
	nod->prev = NULL;

	if ((*lista).head != NULL) {
		(*lista).head->prev = nod;
	}
	else {
		(*lista).tail = nod;
	}
	(*lista).head = nod;
}

ListaDubla inserareFinal(ListaDubla lista, Biblioteca b) {
	Nod* nod = (Nod*)malloc(sizeof(Nod));
	nod->info = b;
	nod->next = NULL;
	nod->prev = lista.tail;

	if (lista.tail != NULL) {
		lista.tail->next = nod;
	}
	else {
		lista.head = nod;
	}
	lista.tail = nod;

	return lista;
}

void afisareListaInceput(ListaDubla lista) {
	while (lista.head != NULL) {
		afisareBiblioteca(lista.head->info);
		lista.head = lista.head->next;
	}
}

void afisareListaFinal(ListaDubla lista) {
	while (lista.tail != NULL) {
		afisareBiblioteca(lista.tail->info);
		lista.tail = lista.tail->prev;
	}
}

void dezalocareLista(ListaDubla* lista) {
	while ((*lista).head != NULL) {
		Nod* aux = (*lista).head;
		(*lista).head = (*lista).head->next;
		free(aux->info.nume);
		free(aux);
	}
	(*lista).head = NULL;
	(*lista).tail = NULL;
}

void stergeNodNume(ListaDubla* lista,const char* nume) {
	if ((*lista).head == NULL && (*lista).tail == NULL)
		printf("Lista este deja goala!");
	else {
		Nod* aux = (*lista).head;

		while (aux->next != NULL && strcmp(aux->info.nume,nume)!=0) {
			aux = aux->next;
		}

		if (aux == lista->head) {
			if (aux == lista->tail) {
				lista->head = NULL;
				lista->tail = NULL;
			}
			else {
				lista->head = lista->head->next;
				lista->head->prev = NULL;
			}
		}
		else if(aux==lista->tail){
			lista->tail = lista->tail->prev;
			lista->tail->next = NULL;
		}
		else {
			aux->next->prev = aux->prev;
			aux->prev->next = aux->next;
		}
		free(aux->info.nume);
		free(aux);

	}
}

void stergeNod(ListaDubla* lista, int index) {
	Nod* aux1 = lista->head;
	Nod* aux2 = lista->head;
	int i = 0;
	if (lista->head == NULL && lista->tail == NULL) {
		printf("Lisa este deja goala!\n");
	}
	else {
		while (aux2 != NULL && i != index) {
			aux1 = aux2;
			aux2 = aux2->next;
			i++;
		}
		if(aux2==NULL) printf("Nu a fost gasit!\n");
		else {
			if (aux2 == lista->head) {
				lista->head = lista->head->next;
				lista->head->prev = NULL;
				free(aux2->info.nume);
				free(aux2);
			}
			else if (aux2 == lista->tail) {
				lista->tail = lista->tail->prev;
				lista->tail->next = NULL;
				free(aux2->info.nume);
				free(aux2);
			}
			else {
				aux1->next = aux2->next;
				aux2->next->prev = aux1;
				free(aux2->info.nume);
				free(aux2);
			}
		}
	}
}

void inserareFisier(ListaDubla* lista) {

	FILE* f = fopen("Biblioteca.txt", "r");
	char buffer[100];
	char delimitator[] = ",\n";

	while (fgets(buffer, 100, f)) {
		Biblioteca b;
		char* token = strtok(buffer, delimitator);
		b.nume = (char*)malloc(sizeof(char) * (strlen(token) + 1));
		strcpy(b.nume, token);

		token = strtok(NULL, delimitator);
		b.nrCarti = atoi(token);
		token = strtok(NULL, delimitator);
		b.nrCititori = atoi(token);

		inserareInceput(lista, b);
	}
	fclose(f);
}

void main() {
	Biblioteca b1 = initializare("Mihai Eminescu", 150, 30);
	Biblioteca b2 = initializare("Ioan Slavici", 200, 30);
	Biblioteca b3 = initializare("Tudor Arghezi", 100, 15);
	Biblioteca b4 = initializare("Radu Tudoran", 660, 65);
	
	ListaDubla lista;
	lista.head = NULL; //head si tail sunt nod*
	lista.tail = NULL;
	lista = inserareFinal(lista, b1);
	lista = inserareFinal(lista, b2);
	lista = inserareFinal(lista, b3);
	lista = inserareFinal(lista, b4);

	afisareListaInceput(lista); printf("\n");
	stergeNodNume(&lista, "Tudor Arghezi");
	afisareListaInceput(lista);
	
	printf("\n==============LISTA2============\n");
	ListaDubla lista2;
	lista2.head = NULL;
	lista2.tail = NULL;
	inserareFisier(&lista2);
	afisareListaInceput(lista2); printf("\nSTERGERE:\n");
	//stergeNod(&lista2, 0);
	afisareListaInceput(lista2);

	dezalocareLista(&lista);
	afisareListaInceput(lista);
	dezalocareLista(&lista2);
}