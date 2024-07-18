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

struct Nod{
	Biblioteca info;
	Nod* next;
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

Nod* inserareInceput(Nod* cap, Biblioteca b) {
	Nod* nod = (Nod*)malloc(sizeof(Nod));
	nod->info = initializare(b.nume, b.nrCarti, b.nrCititori);
	nod->next = cap;

	return nod;
}

void inserareInceputVoid(Nod** cap, Biblioteca b) {
	Nod* nod = (Nod*)malloc(sizeof(Nod));
	nod->info = initializare(b.nume, b.nrCarti, b.nrCititori);
	nod->next = (*cap);
	(*cap) = nod;
}

void inserareSfarsitVoid(Nod** cap, Biblioteca b) {
	Nod* nod = (Nod*)malloc(sizeof(Nod));
	nod->info = initializare(b.nume, b.nrCarti, b.nrCititori);
	nod->next = NULL;

	if ((*cap) != NULL) {
		Nod* aux = (*cap);
		while (aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = nod;
	}
	else (*cap) = nod; //pentru situatia in care ar trebui sa schimb locatia in care pointeaza cap, de aceea folosesc **.
						//daca ipotetic vreau doar sa adaug in plus la un nod deja format, puteam sa folosesc * in loc de ** 
}

Nod* inserareSfarsit(Nod* cap, Biblioteca b) {
	Nod* nod = (Nod*)malloc(sizeof(Nod));
	nod->info = initializare(b.nume, b.nrCarti, b.nrCititori);
	nod->next = NULL;

	Nod* aux = cap;
	if (cap == NULL) return nod;
	else {
		while (aux->next != NULL)
			aux = aux->next;
	}
	aux->next = nod;
	return cap;
}

void adaugareSfarsit(Nod* cap, Biblioteca b) {
	Nod* nod = (Nod*)malloc(sizeof(Nod));
	nod->info = initializare(b.nume, b.nrCarti, b.nrCititori);
	nod->next = NULL;

	while (cap->next != NULL) {
		cap = cap->next;
	}
	cap->next = nod;
}

void adaugareLaPozitie(Nod** cap, Biblioteca b,int poz) {
	Nod* copie = (*cap);
	int dim = 0;
	while (copie != NULL) {
		dim++;
		copie = copie->next;
	}
	if (poz > dim) printf("Nu putem adauga pe aceasta pozitie\n");
	else {
		Nod* aux = (*cap);
		Nod* prev = (*cap);
		Nod* nod = (Nod*)malloc(sizeof(Nod));
		nod->next = NULL;
		nod->info = initializare(b.nume, b.nrCarti, b.nrCititori);
		int i = 0;
		while (aux != NULL && i != poz) {
			prev = aux;
			aux = aux->next;
			i++;
		}
		if (poz == 0) {
			nod->next = (*cap);
			(*cap) = nod;
		}
		else {
			prev->next = nod;
			nod->next = aux;
		}
	}
}

void afisareLista(Nod* nod) {
	while (nod!=NULL)
	{
		afisareBiblioteca(nod->info);
		nod = nod->next;
	}
}

void getBibliotecaMaxCititori(Nod* nod) {
	int max = 0;
	char* nume = NULL;
	while (nod != NULL) {
		if (nod->info.nrCititori > max)
		{
			max = nod->info.nrCititori;
			nume = nod->info.nume;
		}
		nod = nod->next;
	}

	printf("Biblioteca cu cei mai multi cititori este: %s\n", nume);
}

void stergeNod(Nod** nod, int index) {
	Nod* aux1 = (*nod);
	Nod* aux2 = (*nod);
	int i = 0;
	if ((*nod) == NULL) {
		printf("Lisa este deja goala!\n");
	}
	else {
		while (aux2->next != NULL && i != index) {
			aux1 = aux2;
			aux2 = aux2->next;
			i++;
		}
		if (index == 0) {
			(*nod) = (*nod)->next;
			free(aux1->info.nume);
			free(aux1);
		}
		else if (i == index) {
			aux1->next = aux2->next;
			free(aux2->info.nume);
			free(aux2);
		}
		else printf("Nu a fost gasit!\n");
	}
}

void dezalocareLista(Nod** cap) {
	while ((*cap) != NULL) {
		free((*cap)->info.nume);
		Nod* nod = (*cap);
		(*cap) = (*cap)->next;
		free(nod);
	}
}

//FISIER------------------------

Nod* inserareFisier(Nod* cap) {
	
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

		cap = inserareInceput(cap, b);
	}
	fclose(f);
	return cap;
}

Biblioteca* adaugareTXTVector(Biblioteca* vector, int* dim, Biblioteca b) {
	Biblioteca* copie = (Biblioteca*)malloc(sizeof(Biblioteca) * ((*dim)+1));
	
	for (int i = 0; i < (*dim); i++) {
		copie[i] = vector[i];
	}
	copie[*dim] = b;

	if (vector != NULL) {
		free(vector);
	}
	(*dim)++;

	return copie;
}

Nod* inserareOrdonataTXT(Nod* cap) {
	Biblioteca* vector = NULL;
	int dim = 0;

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

		vector = adaugareTXTVector(vector, &dim, b);
	}
	fclose(f);

	for (int i = 0; i < dim - 1; i++) {
		for (int j = i + 1; j < dim; j++) {
			if (vector[i].nrCarti < vector[j].nrCarti) {
				Biblioteca aux = vector[i];
				vector[i] = vector[j];
				vector[j] = aux;
			}
		}
	}

	for(int i=0;i<dim;i++)
		cap = inserareInceput(cap, vector[i]);

	free(vector);
	return cap;
}

Biblioteca* inserareInVector_Conditie(Nod* cap,int nrCarti,int* dim) {
	Nod* aux = cap;
	while (aux != NULL) {
		if (aux->info.nrCarti > nrCarti) {
			(*dim)++;
		}
		aux = aux->next;
	}
	
	Biblioteca* vector = (Biblioteca*)malloc(sizeof(Biblioteca) * (*dim));

	int i = 0;
	while(cap!=NULL){
		if (cap->info.nrCarti > nrCarti) {
			vector[i] = cap->info;
			vector[i].nume = (char*)malloc(sizeof(char) * (strlen(cap->info.nume) + 1));
			strcpy(vector[i].nume, cap->info.nume);
			i++;
		}
		cap = cap->next;
	}

	return vector;
}

void interschimbare(Nod* cap, int poz1, int poz2) {
	Nod* copie = cap;
	Nod* nod1 = cap;
	Nod* nod2 = cap;
	int i = 0;

	if (poz1 > poz2) printf("Pozitia 1 trebuie sa fie mai mica decat pozitia 2\n");
	else if (poz1 == poz2) printf("Nu trebuie schimbat nimic!\n");
	else {
		while (copie != NULL) {
			if (i == poz1) {
				nod1 = copie;
			}
			if (i == poz2) {
				nod2 = copie;
			}
			i++;
			copie = copie->next;
		}
		if (i <= poz2)printf("Nu a fost gasita pozitia\n");
		else {
			Biblioteca temp = nod1->info;
			nod1->info = nod2->info;
			nod2->info = temp;
		}
	}

}


void dezalocareVector(Biblioteca** vector, int* dim) {
	for (int i = 0; i < (*dim); i++) {
		free((*vector)[i].nume);
	}
	free(*vector);
	*vector = NULL;
	*dim = 0;
}

void main() {
	Biblioteca b1 = initializare("Mihai Eminescu", 150, 30);
	Biblioteca b2 = initializare("Ioan Slavici", 200, 30);
	Biblioteca b3 = initializare("Tudor Arghezi", 100, 15);
	Biblioteca b4 = initializare("Radu Tudoran", 660, 65);
	/*afisareBiblioteca(b1);
	afisareBiblioteca(b2);
	afisareBiblioteca(b3);*/

	Nod* cap = NULL;
	/*cap = inserareInceput(cap, b1);
	cap = inserareInceput(cap, b2);
	cap = inserareInceput(cap, b3);*/
	inserareInceputVoid(&cap, b1);
	inserareInceputVoid(&cap, b2);
	inserareInceputVoid(&cap, b3);
	adaugareSfarsit(cap, b4);
	afisareLista(cap); printf("\n");

	/*Nod* cap2 = NULL;
	inserareSfarsitVoid(&cap2, b4);
	afisareLista(cap2);*/

	/*Nod* cap3 = NULL;
	cap3 = inserareSfarsit(cap3, b1);
	cap3 = inserareSfarsit(cap3, b2);
	cap3 = inserareSfarsit(cap3, b3);
	afisareLista(cap3);*/
	getBibliotecaMaxCititori(cap); printf("\n");
	stergeNod(&cap, 4);
	afisareLista(cap);

	printf("\n=================CAP2=================\n");
	Nod* cap2 = NULL;
	cap2 = inserareFisier(cap2);
	afisareLista(cap2);

	printf("\n=================CAP3=================\n");
	Nod* cap3 = NULL;
	cap3 = inserareOrdonataTXT(cap3);
	afisareLista(cap3);

	printf("\n=================VECTOR=================\n");
	int dimVector = 0;
	Biblioteca* biblioteca = inserareInVector_Conditie(cap, 180,&dimVector);
	for (int i = 0; i < dimVector; i++)
		afisareBiblioteca(biblioteca[i]);

	printf("\n=================INTERSCHIMBARE=================\n");
	interschimbare(cap3, 0, 4);
	afisareLista(cap3);

	printf("\n=================Adaugare in mijloc=================\n");
	Biblioteca test = initializare("zzzzz", 10, 5);
	adaugareLaPozitie(&cap3,test,0);
	afisareLista(cap3);

	dezalocareLista(&cap);
	afisareLista(cap);
	dezalocareLista(&cap2);
	dezalocareLista(&cap3);
	dezalocareVector(&biblioteca,&dimVector);
}