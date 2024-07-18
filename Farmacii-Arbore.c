#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Farmacie Farmacie;

struct Farmacie {
	unsigned short int id;
	char* denumire;
	float cifraAfaceri;
	char* sef;
	unsigned char nrAngajati;
	char* localitate;
};

typedef struct Nod Nod;
struct Nod {
	Farmacie info;
	Nod* st;
	Nod* dr;
};

Farmacie initFarmacie(unsigned short int id, const char* denumire, float cifraAfaceri, const char* sef, unsigned char nrAngajati, const char* localitate) {
	Farmacie f;
	f.id = id;
	f.denumire = (char*)malloc(sizeof(char) * (strlen(denumire) + 1));
	strcpy(f.denumire, denumire);
	f.cifraAfaceri = cifraAfaceri;
	f.sef = (char*)malloc(sizeof(char) * (strlen(sef) + 1));
	strcpy(f.sef, sef);
	f.nrAngajati = nrAngajati;
	f.localitate = (char*)malloc(sizeof(char) * (strlen(localitate) + 1));
	strcpy(f.localitate, localitate);

	return f;
}

void afisareFarmacie(Farmacie f) {
	printf("ID: %d | Den: %s | Cifra: %.2f | Sef: %s | NrAngajati: %d | Localitate: %s\n", 
		f.id,f.denumire,f.cifraAfaceri,f.sef,f.nrAngajati,f.localitate);

}

void afisareArbore(Nod* rad) {
	if (rad) {
		afisareArbore(rad->st);
		afisareFarmacie(rad->info);
		afisareArbore(rad->dr);
	}
}

Nod* inserareArbore(Nod* rad, Farmacie f) {
	if (rad) {
		if (rad->info.id > f.id) {
			rad->st = inserareArbore(rad->st, f);
		}
		else {
			rad->dr = inserareArbore(rad->dr, f);
		}
		return rad;
	}
	else {
		Nod* nod = (Nod*)malloc(sizeof(Nod));
		nod->info = f;
		nod->dr = nod->st = NULL;
		return nod;
	}
}

Nod* inserareDinFisier(Nod* cap, int* nr) {
	(*nr) = 0;
	FILE* f = fopen("Farmacii.txt", "r");
	if (f) {
		char buffer[100];
		char delimitator[] = ",\n";

		while (fgets(buffer, 100, f)) {
			Farmacie f;
			char* token = strtok(buffer, delimitator);
			f.id = (unsigned short int)atoi(token);
			
			token = strtok(NULL, delimitator);
			f.denumire = (char*)malloc(sizeof(char) * (strlen(token) + 1));
			strcpy(f.denumire, token);
			
			token = strtok(NULL, delimitator);
			f.cifraAfaceri = atof(token);

			token = strtok(NULL, delimitator);
			f.sef = (char*)malloc(sizeof(char) * (strlen(token) + 1));
			strcpy(f.sef, token);

			token = strtok(NULL, delimitator);
			f.nrAngajati = (unsigned char)atoi(token);

			token = strtok(NULL, delimitator);
			f.localitate = (char*)malloc(sizeof(char) * (strlen(token) + 1));
			strcpy(f.localitate, token);

			(*nr)++;
			cap = inserareArbore(cap, f);
		}
		fclose(f);
	} 
	return cap;
}

void totalAngajatiDupaLocalitate(Nod* rad, const char* localitate, int* total) {
	if (rad) {
		totalAngajatiDupaLocalitate(rad->st, localitate, total);
		totalAngajatiDupaLocalitate(rad->dr, localitate, total);

		if (strcmp(rad->info.localitate, localitate) == 0) {
			(*total) += rad->info.nrAngajati;
		}

	}
}

int totalAng(Nod* rad, const char* localitate) {
	if (rad) {
		int total=0;
		total += totalAng(rad->st, localitate);
		total += totalAng(rad->dr, localitate);

		if (strcmp(rad->info.localitate, localitate) == 0) {
			total += rad->info.nrAngajati;
		}

		return total;
	}
	return 0;
}

float totalCifraAfaceriDupaId(Nod* rad, int id) {
	if (rad) {
		if (rad->info.id == id) {
			return rad->info.cifraAfaceri;
		}
		else if (rad->info.id > id) {
			return totalCifraAfaceriDupaId(rad->st, id);
		}
		else return totalCifraAfaceriDupaId(rad->dr, id);
	}
	return 0;
}

float cifraDeAfaceriMedieLunaraDupaId(Nod* rad, int id,int nr) {
	float cifraAnuala = totalCifraAfaceriDupaId(rad, id);
	return (cifraAnuala / 12) / nr;
}

void dezalocareArbore(Nod** rad) {
	if (*rad) {
		dezalocareArbore((&(*rad)->st));
		dezalocareArbore((&(*rad)->dr));
		free((*rad)->info.denumire);
		free((*rad)->info.localitate);
		free((*rad)->info.sef);
		free(*rad);
		(*rad) = NULL;
	}
}

int inaltimeArbore(Nod* rad) {
	if (rad) {
		int max = 0;
		int st = inaltimeArbore(rad->st);
		int dr = inaltimeArbore(rad->dr);
		if (st > dr) max = st;
		else max = dr;
		return max + 1;
	}
	else return 0;
}

void stergereNoduriFrunzaDupaNivel(Nod** rad, int nivel,int inaltime) {
	if (*rad) {
		if (inaltime == nivel && (*rad)->st==NULL && (*rad)->dr==NULL) {
			dezalocareArbore(rad);
		}
		else {
			stergereNoduriFrunzaDupaNivel(&((*rad)->st), nivel, inaltime+1); //pun & pt ca la rad.st se afla un pointer, nu **
			stergereNoduriFrunzaDupaNivel(&((*rad)->dr), nivel, inaltime+1);
		}
	}
}

typedef struct NodSimplu NodSimplu;
struct NodSimplu {
	Farmacie info;
	Nod* next;
};

NodSimplu* inserareInceputListaSimpla(NodSimplu* nod, Farmacie f) {
	NodSimplu* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = initFarmacie(f.id,f.denumire,f.cifraAfaceri,f.sef,f.nrAngajati,f.localitate);
	nou->next = nod;
	return nou;
}

NodSimplu* generareListaSimpla(Nod* rad, int nivel, int inaltime, NodSimplu* nod) {
	if (rad) {
		if (nivel == inaltime) {
			nod = inserareInceputListaSimpla(nod, rad->info);
		}
		else {
			nod = generareListaSimpla(rad->st, nivel, inaltime + 1, nod);
			nod = generareListaSimpla(rad->dr, nivel, inaltime + 1, nod);
		}
	}
	return nod;
}

void afisareListaSimpla(NodSimplu* nod) {
	while (nod != NULL) {
		afisareFarmacie(nod->info);
		nod = nod->next;
	}
}

void dezalocareListaSimpla(NodSimplu** nod) {
	while (*nod != NULL) {
		NodSimplu* aux = (*nod);
		(*nod) = (*nod)->next;
		free(aux->info.denumire);
		free(aux->info.localitate);
		free(aux->info.sef);
		free(aux);
	}
}

void main() {
	//Farmacie f1 = initFarmacie(1, "Catena", 400.2, "Ion", 5, "Otopeni");
	//afisareFarmacie(f1);

	Nod* rad = NULL;
	int nr = 0;
	rad = inserareDinFisier(rad,&nr);
	afisareArbore(rad);

	int total = 0;
	printf("\n***********\n");
	totalAngajatiDupaLocalitate(rad, "Otopeni", &total);
	printf("Total angajati dupa localitate: %d\n", total);

	//int a = 0;
	//unsigned char b = '4';
	//a += atoi(&b);
	//printf("%d", a);
	printf("\n***********\n");
	//printf("%.2f", totalCifraAfaceriDupaId(rad, 5));
	printf("%.2f", cifraDeAfaceriMedieLunaraDupaId(rad, 5, nr));
	printf("\n***********\n");
	printf("%d", totalAng(rad, "Otopeni"));

	printf("\n***********\n");
	printf("%d", inaltimeArbore(rad));

	printf("\n***********\n");
	stergereNoduriFrunzaDupaNivel(&rad, 2,0);
	afisareArbore(rad);
	printf("\n***********\n");
	NodSimplu* nodsimplu = NULL;
	nodsimplu = generareListaSimpla(rad, 1, 0, nodsimplu);
	afisareListaSimpla(nodsimplu);

	dezalocareArbore(&rad);
	afisareArbore(rad);
	dezalocareListaSimpla(&nodsimplu);
	afisareListaSimpla(nodsimplu);
}

/* Farmacii.txt
5,Catena,400.2,Ion,5,Otopeni
6,Brico,500.44,Julius,10,Bucuresti
3,Farmacia3,1000.13,Stan,12,Constanta
8,Farmacia4,150.55, Cristian,4,Otopeni
7,Farmacia5,901.2,Mihai,13,Brasov
4,Farmacia6,440,Paul,6,Iasi*/