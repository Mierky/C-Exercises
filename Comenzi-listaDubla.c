#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Comanda Comanda;
typedef struct Nod Nod;

struct Comanda {
	unsigned int id;
	char* status;
	char* nume_client;
	unsigned char numar_produse;
	float suma_plata;
};

struct Nod {
	Comanda info;
	Nod* next;
	Nod* prev;
};

typedef struct ListaDubla ListaDubla;
struct ListaDubla {
	Nod* head;
	Nod* tail;
};

Comanda initComanda(unsigned int id, const char* status, const char* nume, unsigned char numar, float suma) {
	Comanda c;
	c.id = id;
	c.status = (char*)malloc(sizeof(char) * (strlen(status) + 1));
	strcpy(c.status, status);
	c.nume_client = (char*)malloc(sizeof(char) * (strlen(nume) + 1));
	strcpy(c.nume_client, nume);
	c.numar_produse = numar;
	c.suma_plata = suma;
	return c;
}

ListaDubla inserareLaFinal(ListaDubla lista, Comanda c) {
	Nod* nod = (Nod*)malloc(sizeof(Nod));
	nod->info = c;
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

ListaDubla inserareDinFisier(ListaDubla lista) {
	FILE* f = fopen("Comenzi.txt","r");
	if (f) {
		char buffer[100];
		char delimitator[] = ",\n";
		while (fgets(buffer, 100, f)) {
			Comanda c;
			char* token = strtok(buffer, delimitator);
			c.id = (unsigned int)atoi(token);

			token = strtok(NULL, delimitator);
			c.status = (char*)malloc(sizeof(char) * (strlen(token) + 1));
			strcpy(c.status, token);

			token = strtok(NULL, delimitator);
			c.nume_client = (char*)malloc(sizeof(char) * (strlen(token) + 1));
			strcpy(c.nume_client, token);

			token = strtok(NULL, delimitator);
			c.numar_produse = (unsigned char)atoi(token);

			token = strtok(NULL, delimitator);
			c.suma_plata = atof(token);

			lista = inserareLaFinal(lista, c);
		}
		fclose(f);
	}
	return lista;
}

void afisareComanda(Comanda c) {
	printf("%d | %s | %s | %d | %.2f\n", c.id, c.status, c.nume_client, c.numar_produse, c.suma_plata);
}

void afisareListaInceput(ListaDubla nod) {
	while (nod.head != NULL) {
		afisareComanda(nod.head->info);
		nod.head = nod.head->next;
	}
}

float sumaPretComenziDupaStatus(ListaDubla lista,const char* status) {
	float pret = 0;
	int i = 0;
	while (lista.head != NULL) {
		if (strcmp(lista.head->info.status, status) == 0) {
			pret += lista.head->info.suma_plata;
			i++;
		}
		lista.head = lista.head->next;
	}
	if (i != 0) { return pret/i; }
	else return 0;
}

void mediePretDupaStatus(ListaDubla lista, const char* status) {
	float pret = sumaPretComenziDupaStatus(lista, status);
	printf("Pretul mediu al produselor cu statusul '%s' = %.2f", status, pret);
}

void identificareIdDuplicat(ListaDubla lista) {
	int i = 0;
	while (lista.head != NULL) {
		Nod* nod = lista.head->next;
		while (nod != NULL) {
			if (lista.head->info.id == nod->info.id) {
				i++;
				break;
			}
			nod = nod->next;
		}
		lista.head = lista.head->next;
	}
	if (i != 0) {
		printf("S-a gasit %d dublicat!\n", i);
	}
	else printf("Nu s-a gasit duplicat!");
}

int comparator(Comanda comanda1, Comanda comanda2) {
	return comanda1.id == comanda2.id && comanda1.numar_produse == comanda2.numar_produse
		&& comanda1.suma_plata == comanda2.suma_plata && !strcmp(comanda1.nume_client,comanda2.nume_client)
		&& !strcmp(comanda1.status,comanda2.status);
}

int numarClientiUnici(ListaDubla lista) {
	int i = 0;
	while (lista.head != NULL) {
		int unic = 1;
		Nod* nod = lista.tail;
		while (nod != lista.head) {
			if (comparator(lista.head->info, nod->info)) {
				unic = 0;
				break;
			}
			nod = nod->prev;
		}
		if (unic) i++;
		lista.head = lista.head->next;
	}
	return i;
}

Nod* inserareInArbore(Nod* rad, Comanda c) {
	if (rad) {
		if (rad->info.id > c.id) {
			rad->prev = inserareInArbore(rad->prev, c);
		}
		else {
			rad->next = inserareInArbore(rad->next, c);
		}

		return rad;
	}
	else {
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = c; //shallow
		nou->next = nou->prev = NULL;
		return nou;
	}
}

Nod* adaugaInInserareArbore(Nod* nod) {
	Nod* arbore = NULL;
	while (nod != NULL) {
		arbore = inserareInArbore(arbore, nod->info);
		nod = nod->next;
	}
	return arbore;
}

void afisareArbore(Nod* nod) {
	if (nod) {
		afisareArbore(nod->prev);
		afisareComanda(nod->info);
		afisareArbore(nod->next);
	}
}

void dezalocare(ListaDubla* lista) {
	while ((*lista).head != NULL) {
		Nod* aux = lista->head;
		lista->head = lista->head->next;
		free(aux->info.nume_client);
		free(aux->info.status);
		free(aux);
	}
	lista->head = NULL;
	lista->tail = NULL;
}

void main() {
	//Comanda c1 = initComanda(2, "bine", "diana", 23, 22.4);
	//Comanda c2 = initComanda(2, "bin", "diana", 23, 22.4);
	//printf("%d", comparator(c1, c2));
	ListaDubla lista;
	lista.head = NULL;
	lista.tail = NULL;
	lista = inserareDinFisier(lista);
	afisareListaInceput(lista);
	mediePretDupaStatus(lista, "livrata");
	printf("\n*********\n");
	identificareIdDuplicat(lista);
	printf("\n*********\n");
	printf("Numarul de clienti unici: %d\n",numarClientiUnici(lista));
	Nod* arbore = NULL;
	arbore = adaugaInInserareArbore(lista.head);
	afisareArbore(arbore);
	dezalocare(&lista);
	afisareListaInceput(lista);
}

/*
4,La depozit,Dana,8,103.5
1,In drum spre casa,Marius,2,50
2,La depozit,Ion,10,550.45
6,livrata,Stan,1,88.21
3,livrata,Gabriel,4,345.22
5,In tranzit,Ionela,3,70
*/