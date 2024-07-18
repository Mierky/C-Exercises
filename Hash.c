#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

typedef struct HashTable HashTable;
typedef struct Nod Nod;

typedef struct {
	int id;
	int an;
	int etaje;
}Cladire;

struct Nod {
	Cladire info;
	Nod* next;
};

struct HashTable {
	int dim;
	Nod** vector;
};

Cladire initializareCladire(int id, int an, int etaje) {
	Cladire c;
	c.id = id;
	c.an = an;
	c.etaje = etaje;
	return c;
}

HashTable initializareHash(int dim) {
	HashTable hash;
	hash.dim = dim;
	hash.vector = (Nod**)malloc(sizeof(Nod*) * dim);

	for (int i = 0; i < dim; i++)
		hash.vector[i] = NULL;
	
	return hash;
}

int generareCodHash(int an,int id,int dim) {
	if (dim > 0) {
		int pozitie = id * an;
		return pozitie % dim;
	}
	return -1;
}

void inserareNodSfarsit(Nod** nod,Cladire c) {
	Nod* temp = (Nod*)malloc(sizeof(Nod));
	temp->info = initializareCladire(c.id, c.an, c.etaje);
	temp->next = NULL;
	
	if ((*nod) == NULL) 
		(*nod) = temp;
	else {
		Nod* aux = (*nod);
		while (aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = temp;
	}
}

void inserareTabelaHash(HashTable table, Cladire c) {
	if (table.dim > 0) {
		int pozitie = generareCodHash(c.an, c.id, table.dim);
		inserareNodSfarsit(&(table.vector[pozitie]), c);
	}
}

void afisareCladire(Cladire c) {
	printf("Cladirea cu id-ul %d a fost realizata in anul %d si are %d etaje\n", c.id, c.an, c.etaje);
}

void afisareLista(Nod* nod) {
	while (nod != NULL) {
		afisareCladire(nod->info);
		nod = nod->next;
	}
}

void afisareHashTable(HashTable hash) {
	for (int i = 0; i < hash.dim; i++) {
		printf("\nCLUSTER %d:\n", i);
		if (hash.vector[i] != NULL)
			afisareLista(hash.vector[i]);
		else printf("- - - - - -\n");
	}
}

void afisareCluster(HashTable hash,int an,int id) {
	int pozitie = generareCodHash(an, id, hash.dim);
	printf("ELEMENTELE DIN CLUSTER:\n");
	afisareLista(hash.vector[pozitie]);
}

void stergereCladire(int id, int an, HashTable hash) {
	int pozitie = generareCodHash(an, id, hash.dim);
	if (hash.vector[pozitie] == NULL) printf("Clusterul este deja gol!");
	else {
		Nod* aux1 = hash.vector[pozitie];
		Nod* aux2 = hash.vector[pozitie];

		while (aux2 != NULL && aux2->info.id != id && aux2->info.an != an) {
			aux1 = aux2;
			aux2 = aux2->next;
		}
		if(aux2==NULL){
			printf("Nu exista aceasta cladire");
		}
		else if(aux1==aux2){
			hash.vector[pozitie] = hash.vector[pozitie]->next;
			free(aux2);
		}
		else{
			aux1->next = aux2->next;
			free(aux2);
		}
	}
}

void stergereCladireDupaID(int id, HashTable hash) {
	for (int i = 0; i < hash.dim; i++) {
		if (hash.vector[i] != NULL) {

			Nod* aux1 = hash.vector[i];
			Nod* aux2 = hash.vector[i];

			while (aux2 != NULL && aux2->info.id != id) {
				aux1 = aux2;
				aux2 = aux2->next;
			}
			if (aux2 == NULL) {
				printf("Nu exista aceasta cladire in clusterul %d\n",i);
			}
		    else if (aux1 == aux2) {
				hash.vector[i] = hash.vector[i]->next;
				free(aux2);
			}
			else {
				aux1->next = aux2->next;
				free(aux2);
			}
		}
		else printf("Clusterul %d este gol\n", i);
	}
}

void cautareAn(Nod* cap, int an, int* dim) {
	while (cap != NULL) {
		if (cap->info.an == an) (*dim)++;

		cap = cap->next;
	}
}

Cladire* inserareVector(int an, HashTable hash,int* dim) {
	for (int i = 0; i < hash.dim; i++) {
		Nod* nod = hash.vector[i];
		if (hash.vector[i] != NULL)
			cautareAn(nod, an, dim);
	}

	int j = 0;
	Cladire* vector = (Cladire*)malloc(sizeof(Cladire) * (*dim));
	for (int i = 0; i < hash.dim; i++) {
		Nod* nod = hash.vector[i];
		if (nod != NULL) {
			while (nod != NULL) {
				if (nod->info.an == an) {
					vector[j] = initializareCladire(nod->info.id, nod->info.an, nod->info.etaje);
					j++;
				}
				nod = nod->next;
			}
		}
	}

	return vector;
}

void modificareCladire_an(int id, int anNou, int anVechi, HashTable hash) {
	int pozitie = generareCodHash(anVechi, id, hash.dim);
	
	Nod* nod = hash.vector[pozitie];
	Cladire c;
	while (nod != NULL && nod->info.id != id) {
		nod = nod->next;
	}
	if (nod == NULL) printf("Cladirea nu a fost gasita\n");
	else {
		c.id = nod->info.id;
		c.an = anNou;
		c.etaje = nod->info.etaje;
		stergereCladire(id, anVechi, hash);
		inserareTabelaHash(hash, c);
	}
}

void dezalocareNod(Nod** nod) {
	while ((*nod) != NULL) {
		Nod* aux = *nod;
		(*nod) = (*nod)->next;
		free(aux);
	}
}

void dezalocareHashTable(HashTable* hash) {
	for (int i = 0; i < (*hash).dim; i++) {
		if ((*hash).vector[i] != NULL) {
			dezalocareNod(&((*hash).vector[i]));
			(*hash).vector[i] = NULL;
		}
	}
	(*hash).dim = 0;
}

void main() {
	Cladire c1 = initializareCladire(1, 2020, 4);
	Cladire c2 = initializareCladire(2, 2024, 1);
	Cladire c3 = initializareCladire(3, 2018, 6);
	Cladire c4 = initializareCladire(4, 2020, 8);
	Cladire c5 = initializareCladire(5, 2011, 4);

	HashTable hash = initializareHash(4);
	inserareTabelaHash(hash, c1);
	inserareTabelaHash(hash, c2);
	inserareTabelaHash(hash, c3);
	inserareTabelaHash(hash, c4);
	inserareTabelaHash(hash, c5);

	afisareHashTable(hash);
	printf("============\n");
	afisareCluster(hash, 2020, 1);
	/*printf("====STERGERE========\n");
	stergereCladire(1, 2020, hash);
	afisareHashTable(hash);*/
	/*printf("====STERGERE2========\n");
	stergereCladireDupaID(3, hash);
	stergereCladireDupaID(2, hash);
	afisareHashTable(hash);*/

	printf("=======MODIFICARI========\n");
	modificareCladire_an(2, 3000, 2024, hash);
	afisareHashTable(hash);

	printf("=======AFISARE VECTOR========\n");
	int dim = 0;
	Cladire* vector = inserareVector(2020, hash,&dim);
	for (int i = 0; i < dim; i++) {
		afisareCladire(vector[i]);
}
	afisareHashTable(hash);

	printf("=======DEZALOCARE========\n");
	dezalocareHashTable(&hash);
	afisareHashTable(hash);
}