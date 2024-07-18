#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Rezervare Rezervare;
struct Rezervare {
	unsigned int id;
	char* denumire;
	unsigned char categorie;
	char* nume_client;
	unsigned char numar_zile;
	float suma_plata;
};

typedef struct Nod Nod;
struct Nod {
	Rezervare info;
	Nod* next;
};

typedef struct HashTable HashTable;
struct HashTable {
	int dim;
	Nod** vector;
};

Rezervare initRezervare(unsigned int id, const char* denumire, unsigned char categorie, const char* nume, unsigned char numar, float suma) {
	Rezervare r;
	r.id = id;
	r.denumire = (char*)malloc(sizeof(char) * (strlen(denumire) + 1));
	strcpy(r.denumire, denumire);
	r.categorie = categorie;
	r.nume_client = (char*)malloc(sizeof(char) * (strlen(nume) + 1));
	strcpy(r.nume_client, nume);
	r.numar_zile = numar;
	r.suma_plata = suma;

	return r;
}

HashTable initHash(int dim) {
	HashTable hash;
	hash.dim = dim;
	hash.vector = (Nod**)malloc(sizeof(Nod*) * dim);
	for (int i = 0; i < dim; i++) {
		hash.vector[i] = NULL;
	}
	return hash;
}


int generareCodHash(unsigned char categorie, int dim) {
	if (dim > 0) {
		int nr = categorie;
		return nr % dim;
	}
	return -1;
}

void inserareLaFinal(Nod** nod, Rezervare r) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = r;
	nou->next = NULL;

	if (*nod) {
		Nod* aux = (*nod);
		while (aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = nou;
	}
	else (*nod) = nou;
}

void adaugaRezervareInHash(HashTable hash, Rezervare r) {
	if (hash.dim > 0) {
		int poz = generareCodHash(r.categorie, hash.dim);
		inserareLaFinal(&(hash.vector[poz]), r);
	}
}

HashTable inserareDinFisier(HashTable hash) {

	FILE* f = fopen("Rezervari.txt", "r");
	if (f) {
		char buffer[100];
		char delimitator[] = ",\n";
		while (fgets(buffer, 100, f)) {
			Rezervare r;
			char* token = strtok(buffer, delimitator);
			r.id = (unsigned int)atoi(token);

			token = strtok(NULL, delimitator);
			r.denumire = (char*)malloc(sizeof(char) * (strlen(token) + 1));
			strcpy(r.denumire, token);

			token = strtok(NULL, delimitator);
			r.categorie = (unsigned char)token[0];

			token = strtok(NULL, delimitator);
			r.nume_client = (char*)malloc(sizeof(char) * (strlen(token) + 1));
			strcpy(r.nume_client, token);

			token = strtok(NULL, delimitator);
			r.numar_zile = (unsigned char)atoi(token);

			token = strtok(NULL, delimitator);
			r.suma_plata = atof(token);

			adaugaRezervareInHash(hash, r);
		}
		fclose(f);
	}
	return hash;
}

void afisareRezervare(Rezervare r) {
	printf("%d | %s | %c | %s | %d | %.2f\n", r.id, r.denumire, r.categorie, r.nume_client, r.numar_zile, r.suma_plata);
}

void afisareHash(HashTable h) {
	for (int i = 0; i < h.dim; i++) {
		printf("\n----CLUSTER %d----\n", i);
		Nod* nod = h.vector[i];
		while (nod != NULL) {
			afisareRezervare(nod->info);
			nod = nod->next;
		}
	}
}

int numarRezervariDupaCategorie(HashTable hash, unsigned char categorie) {
	int nrRezervari = 0;
	int poz = generareCodHash(categorie, hash.dim);
	Nod* nod = hash.vector[poz];

	while (nod) {
		nrRezervari++;
		nod = nod->next;
	}

	return nrRezervari;
}

void stergereRezervare(HashTable hash, unsigned char categorie,unsigned int id) {
	int poz = generareCodHash(categorie, hash.dim);
	Nod* nod = NULL;
	Nod* next = hash.vector[poz];

	while (next != NULL && next->info.id != id) {
		nod = next;
		next = next->next;
	}
	if (nod == NULL) {
		hash.vector[poz] = hash.vector[poz]->next;
		free(next);
	}
	else if (next != NULL) {
		nod->next = next->next;
		free(next);
	}

}

void modificareCategorieDupaNume(HashTable hash, const char* denumire, unsigned char categorie) {
	for (int i = 0; i < hash.dim; i++) {
		Nod* nod = hash.vector[i];

		while (nod != NULL && strcmp(nod->info.denumire, denumire) != 0) {
			nod = nod->next;
		}

		if (nod) {
			Rezervare r = nod->info;
			r.categorie = categorie;
			stergereRezervare(hash, nod->info.categorie,r.id);
			adaugaRezervareInHash(hash, r);
		}
	}
}

void dezalocareHash(HashTable* hash) {
	for (int i = 0; i < hash->dim; i++) {
		while (hash->vector[i]) {
			Nod* aux = hash->vector[i];
			hash->vector[i] = hash->vector[i]->next;
			free(aux->info.denumire);
			free(aux->info.nume_client);
			free(aux);
		}
		hash->vector[i] = NULL;
	}
	hash->dim = 0;
	hash->vector = NULL;
}
/////////////////////
int generareCodDupaNume(const char* nume, int dim) {
	if (dim > 0) {
		int nr = strlen(nume);
		return nr % dim;
	}
	return -1;
}

void adaugaRezervareInHash2(HashTable hash, Rezervare r) {
	if (hash.dim > 0) {
		int poz = generareCodDupaNume(r.nume_client, hash.dim);
		inserareLaFinal(&(hash.vector[poz]), r);
	}
}

HashTable inserareDinHashTable(HashTable t, unsigned char categorie, HashTable hash2) {
	int poz = generareCodHash(categorie, t.dim);
	Nod* nod = t.vector[poz];
	while (nod != NULL) {
		Rezervare r = initRezervare(nod->info.id, nod->info.denumire, nod->info.categorie, nod->info.nume_client, nod->info.numar_zile, nod->info.suma_plata);
		adaugaRezervareInHash2(hash2, r);
		nod = nod->next;
	}

	return hash2;
}

////////////HEAP - dupa suma_plata / trebuie sa aibe memoria ei pentru valorile adaugate
typedef struct Heap Heap;
struct Heap {
	int dim;
	int dimTotala;
	Rezervare* vector;
};

Heap initHeap(int dimTotala) {
	Heap h;
	h.dimTotala = dimTotala;
	h.dim = 0;
	h.vector = (Rezervare*)malloc(sizeof(Rezervare) * dimTotala);

	return h;
}

Heap adaugaValoriInVectorHeap(Heap h, HashTable table) {
	int j = 0;
	for (int i = 0; i < table.dim; i++) {
		Nod* nod = table.vector[i];
		while (nod) {
			h.vector[j] = nod->info;
			j++;
			nod = nod->next;
		}
	}
	h.dim = j;
	return h;
}

void afisareVectorHeap(Heap h) {
	for (int i = 0; i < h.dim; i++) {
		afisareRezervare(h.vector[i]);
	}
}

void filtrareHeap(Heap h, int poz) {
	int poz_s = poz * 2 + 1;
	int poz_d = poz * 2 + 2;
	int poz_max = poz;

	if (poz_s < h.dim && h.vector[poz_max].suma_plata < h.vector[poz_s].suma_plata) {
		poz_max = poz_s;
	}
	if (poz_d < h.dim && h.vector[poz_max].suma_plata < h.vector[poz_d].suma_plata) {
		poz_max = poz_d;
	}

	if (poz_max != poz) {
		Rezervare aux = h.vector[poz_max];
		h.vector[poz_max] = h.vector[poz];
		h.vector[poz] = aux;

		if (poz_max * 2 + 1 < h.dim) {
			filtrareHeap(h, poz_max);
		}
	}
}

void main() {
	//afisareRezervare(initRezervare(1, "Hotel1", 'H', "Ion", 10, 1003.2));

	HashTable hash = initHash(5);
	hash = inserareDinFisier(hash);
	afisareHash(hash);

	printf("\n***********************\n");
	printf("%d", numarRezervariDupaCategorie(hash, 'H'));
	printf("\n***********************\n");
	modificareCategorieDupaNume(hash, "Pensiune2", 'j');
	afisareHash(hash);

	printf("\n************hashtable2**********\n");
	HashTable hash2 = initHash(5);
	hash2 = inserareDinHashTable(hash, 'H', hash2);
	afisareHash(hash2);

	printf("\n************HEAP**********\n");
	Heap h = initHeap(6);
	h = adaugaValoriInVectorHeap(h, hash);
	//printf("%d", h.dim);
	afisareVectorHeap(h);
	for (int i = (h.dim / 2) - 1; i >= 0; i--) {
		filtrareHeap(h, i);
	}
	printf("\n************dupa filtrare**********\n");
	afisareVectorHeap(h);

	printf("\n***********dezalocare************\n");
	dezalocareHash(&hash);
	afisareHash(hash);
}

/*
1,Hotel1,H,Ion,10,1003.2
2,Hotel2,H,Dan,4,550.5
3,Hostel1,h,Cristi,2,340.2
4,Pensiune1,P,Pana,4,430
5,Pensiune2,P,Hana,1,100.4
6,Hotel3,H,Danny,2,250.5
*/