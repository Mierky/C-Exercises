#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Programare Programare;

struct Programare {
	char* data;
	char* nume_client;
	float durata_ore;
	char* servicii;
	int cost;
};
typedef struct NodSecundar NodSecundar;
typedef struct NodPrincipal NodPrincipal;
struct NodPrincipal {
	char* data_info;
	NodSecundar* lista;
	NodPrincipal* next;
};

struct NodSecundar {
	Programare info;
	NodSecundar* next;
};

Programare initProgramare(const char* data, const char* nume, float durata, const char* servicii, int cost) {
	Programare p;
	p.data = (char*)malloc(sizeof(char) * (strlen(data) + 1));
	strcpy(p.data, data);
	p.nume_client = (char*)malloc(sizeof(char) * (strlen(nume) + 1));
	strcpy(p.nume_client, nume);
	p.durata_ore = durata;
	p.servicii = (char*)malloc(sizeof(char) * (strlen(servicii) + 1));
	strcpy(p.servicii, servicii);
	p.cost = cost;

	return p;
}

NodPrincipal* inserareNodPrincipal(NodPrincipal* nod, Programare p) {
	NodPrincipal* nou = (NodPrincipal*)malloc(sizeof(NodPrincipal));
	nou->data_info = p.data; //shallow
	nou->next = NULL;
	nou->lista = NULL;

	if (nod) {
		NodPrincipal* aux = nod;
		while (aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = nou;
		return nod;
	}

	return nou;
}

NodSecundar* inserareNodSecundar(NodSecundar* nod, Programare p) {
	NodSecundar* nou = (NodSecundar*)malloc(sizeof(NodSecundar));
	nou->info = p;
	nou->next = NULL;
	if (nod) {
		NodSecundar* aux = nod;
		while (aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = nou;
		return nod;
	}
	else return nou;
}

NodPrincipal* cautaNodDupaData(NodPrincipal* graf, const char* data) {
	while (graf != NULL && strcmp(graf->data_info, data) != 0) {
		graf = graf->next;
	}
	return graf;
}

NodPrincipal* inserareGrafDinFisier(NodPrincipal* graf) {
	FILE* f = fopen("Programari.txt", "r");
	if (f) {
		char buffer[100];
		char delimitator[] = ",\n";

		while (fgets(buffer, 100, f)) {
			Programare p;
			char* token = strtok(buffer, delimitator);
			p.data = (char*)malloc(sizeof(char) * (strlen(token) + 1));
			strcpy(p.data, token);

			token = strtok(NULL, delimitator);
			p.nume_client = (char*)malloc(sizeof(char) * (strlen(token) + 1));
			strcpy(p.nume_client, token);

			token = strtok(NULL, delimitator);
			p.durata_ore = atof(token);

			token = strtok(NULL, delimitator);
			p.servicii = (char*)malloc(sizeof(char) * (strlen(token) + 1));
			strcpy(p.servicii, token);

			token = strtok(NULL, delimitator);
			p.cost = atoi(token);

			if (cautaNodDupaData(graf, p.data) == NULL) {
				graf = inserareNodPrincipal(graf, p);
			}
			NodPrincipal* nod = cautaNodDupaData(graf, p.data);
			if (nod) {
				nod->lista = inserareNodSecundar(nod->lista, p);
			}
		}

		fclose(f);
	}
	return graf;
}

void afisareListaPrincipala(NodPrincipal* nod) {
	while (nod) {
		printf("%s | ", nod->data_info);
		nod = nod->next;
	}
}

void afisareProgramare(Programare p) {
	printf("%s ~ %s ~ %.2f ~ %s ~ %d\n", p.data, p.nume_client, p.durata_ore, p.servicii, p.cost);
}

void afisareListaSecundara(NodSecundar* nod) {
	while (nod) {
		afisareProgramare(nod->info);
		nod = nod->next;
	}
}

int costTotalDupaData(NodPrincipal* graf, const char* data) {
	int total = 0;
	graf = cautaNodDupaData(graf, data);
	NodSecundar* nod = graf->lista;
	while (nod) {
		total += nod->info.cost;
		nod = nod->next;
	}

	return total;
}

void modificareCostProgramare(NodPrincipal* graf, const char* nume, int cost) {
	while (graf) {
		NodSecundar* nod = graf->lista;
		while (nod && strcmp(nod->info.nume_client, nume) != 0) {
			nod = nod->next;
		}
		if (nod) {
			nod->info.cost = cost;
		}
		graf = graf->next;
	}
}

int numarProgramari(NodSecundar* nod) {
	int i = 0;
	while (nod != NULL) {
		nod = nod->next;
		i++;
	}
	return i;
}

void stergeProgramareDupaOra(NodPrincipal* graf, float ora) {
	while (graf) {
		NodSecundar* nodnext = graf->lista;
		NodSecundar* nodprev = NULL;
		while (nodnext != NULL) {
			if (nodnext->info.durata_ore < ora) {
				if (nodprev == NULL) {
					// Deleting the first node in the list
					graf->lista = nodnext->next;
				}
				else {
					// Deleting a node that's not the first in the list
					nodprev->next = nodnext->next;
				}

				// Freeing memory
				free(nodnext->info.nume_client);
				free(nodnext->info.data);
				free(nodnext->info.servicii);
				NodSecundar* temp = nodnext;
				nodnext = nodnext->next;
				free(temp);
			}
			else {
				nodprev = nodnext;
				nodnext = nodnext->next;
			}
		}

		graf = graf->next;
	}
}

NodSecundar* push(NodSecundar* nod,Programare p) {
	NodSecundar* nou = (NodSecundar*)malloc(sizeof(NodSecundar));
	nou->info = p;
	nou->next = nod;
	return nou;
}

NodSecundar* adaugaInStivaDinLista(NodPrincipal* graf) {
	NodSecundar* stiva = NULL;
	while (graf) {
		NodSecundar* nod = graf->lista;
		while (nod) {
			stiva = push(stiva,nod->info);
			nod = nod->next;
		}

		graf = graf->next;
	}
	return stiva;
}

Programare pop(NodSecundar** nod) {
	if (*nod) {
		Programare p = (*nod)->info;
		NodSecundar* aux = *nod;
		*nod = (*nod)->next;
		free(aux);
		return p;
	}
	return initProgramare("N/A", "N/A", 0.0, "N/A", 0);
}

void afisareStiva(NodPrincipal* graf,NodSecundar** stiva) {
	while (graf) {
		NodSecundar* nod = graf->lista;
		while (nod) {
			Programare p = pop(&nod);
			*stiva = push(*stiva, p);
		}
		while (*stiva) {
			Programare p = pop(stiva);
			afisareProgramare(p);
		}
		graf = graf->next;
	}
}

void dezalocare(NodSecundar** nod) {
	while (*nod) {
		NodSecundar* aux = (*nod);
		free(aux->info.data);
		free(aux->info.nume_client);
		free(aux->info.servicii);
		free(aux);
		(*nod) = (*nod)->next;
	}
}

void main() {
	/*Programare p = initProgramare("01-02-2020", "ion", 2.3, "da|nu", 200);
	afisareProgramare(p);*/
	NodPrincipal* graf = NULL;
	graf = inserareGrafDinFisier(graf);
	printf("----LISTA PRINCIPALA----\n");
	afisareListaPrincipala(graf);
	printf("\n----LISTA SECUNDARA (02-02-2021)----\n");
	NodPrincipal* nod = cautaNodDupaData(graf, "02-02-2021");
	if (nod) {
		afisareListaSecundara(nod->lista);
	}

	printf("\n******************\nTOTAL: %d", costTotalDupaData(graf, "02-02-2021"));
	printf("\n******************\n");
	modificareCostProgramare(graf, "Paulina", 200);
	afisareListaSecundara(nod->lista);
	printf("\n******************\n");
	stergeProgramareDupaOra(graf, 1);
	NodPrincipal* nod1 = cautaNodDupaData(graf, "01-01-2020");
	if (nod1) {
		afisareListaSecundara(nod1->lista);
	}

	printf("\n********STIVA*********\n");
	NodSecundar* stiva = adaugaInStivaDinLista(graf);
	afisareListaSecundara(stiva);

	printf("\n********STIVA2*********\n");
	NodSecundar* stiva2 = NULL;
	afisareStiva(graf, &stiva2); 

	/*
	01-01-2020,Ion,1,Spalare|Coafare,100
	02-02-2021,Paulina,0.3,Curatare,40
	03-03-2022,John,1.45,Spalare|Curatare|Masaj,400
	04-04-2023,Claudia,1.2,Tratare dinti,140
	05-05-2024,Daria,2,Masaj|Inot,500
	01-01-2020,Copie1,0.4,Spalare|Coafare,100
	01-01-2020,Copie2,1,Spalare|Coafare,100
	02-02-2021,Copie1,0.3,Curatare,40
	*/
}