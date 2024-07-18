#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
	char* serie;
	float pret;
	int nrPorturi;
}Calculator;

void afisare(Calculator c) {
	printf("Seria: %s\n",c.serie);
	printf("Pret: %.2f\n", c.pret);
	printf("NrPorturi: %d\n\n", c.nrPorturi);
}

Calculator initializare(const char* serie, float pret, int nrP) {
	Calculator c;
	c.pret = pret;
	c.nrPorturi = nrP;
	c.serie = (char*)malloc(sizeof(char) * strlen(serie) + 1);
	strcpy(c.serie, serie);
	return c;
}

void citire(Calculator* c) {
	printf("Seria: ");
	c->serie = (char*)malloc(100 * sizeof(char));
	gets(c->serie);
	printf("Pret: ");
	scanf("%f", &c->pret);
	printf("NrPorturi: ");
	scanf("%d", &c->nrPorturi);
}

void adaugaCalculatorInVector(Calculator c, Calculator* vector) {
	*vector = initializare(c.serie, c.pret, c.nrPorturi);
}

Calculator* adaugaCalculatorInVectorFaraDimensiune(Calculator c, Calculator* vector,int* dim) {
	Calculator* copie = (Calculator*)malloc(sizeof(Calculator) * ((*dim)+1));
	
	for (int i = 0; i < (*dim); i++) {
		copie[i] = vector[i];
	}
	copie[*dim] = c;

	if (vector != NULL) {
		free(vector);
	}
	
	(*dim)++;

	return copie;
}

void afisareVector(Calculator* v, int dim) {
	for (int i = 0; i < dim; i++) {
		afisare(v[i]);
	}
}

Calculator* copieVector(Calculator* vector,int dim) {
	Calculator* copie = (Calculator*)malloc(sizeof(Calculator) * dim);

	for (int i = 0; i < dim; i++) {
		/*copie[i] = vector[i];
		copie[i].serie = malloc(strlen(vector[i].serie) + 1);
		strcpy(copie[i].serie, vector[i].serie);*/
		copie[i] = initializare(vector[i].serie, vector[i].pret, vector[i].nrPorturi);
	}

	return copie;
}

void copieVectorTEST(Calculator* vector, int dim,Calculator* TEST) {
	for (int i = 0; i < dim; i++) {
		/*copie[i] = vector[i];
		copie[i].serie = malloc(strlen(vector[i].serie) + 1);
		strcpy(copie[i].serie, vector[i].serie);*/
		TEST[i] = initializare(vector[i].serie, vector[i].pret, vector[i].nrPorturi);
	}
}

Calculator* concatenare(Calculator* c1, Calculator* c2, int dim1, int dim2,int* dimC) {
	(*dimC) = dim1 + dim2;
	Calculator* concatenat = (Calculator*)malloc(sizeof(Calculator) * (*dimC));
	for (int i = 0; i < dim1; i++) {
		concatenat[i] = c1[i];
		concatenat[i].serie = (char*)malloc(sizeof(char) * (strlen(c1[i].serie) + 1));
		strcpy(concatenat[i].serie, c1[i].serie);
	}
	for (int i = 0; i < dim2; i++) {
		concatenat[dim1] = c2[i];
		concatenat[dim1].serie = (char*)malloc(sizeof(char) * (strlen(c2[i].serie) + 1));
		strcpy(concatenat[dim1].serie, c2[i].serie);
		dim1++;
	}
	return concatenat;
}

void dezalocare(Calculator** vector,int* dim) { 
	for (int i = 0; i < (*dim); i++) {
		free((*vector)[i].serie);
	}
	free(*vector);
	*vector = NULL; //** cand schimb adresa
	*dim = 0;
}

//FISIERE-----------------------------------------
void adaugareTXT(Calculator c,FILE* ft, int* nr) {
	fprintf(ft, "%s\n", c.serie);
	fprintf(ft, "%.2f\n", c.pret);
	fprintf(ft, "%d\n", c.nrPorturi);
	(*nr)++;
}

void afisareTXT(const char* numeFisier){
	FILE* ft = fopen(numeFisier, "r");
	char buffer[100];
	while (fgets(buffer,100,ft)) {
		printf("%s", buffer);
	}
	fclose(ft);
}

void citireTXT(const char* numeFisier, int dim,Calculator* vector) {
	FILE* ft = fopen(numeFisier, "r");
	if (!ft) printf("Eroare la deschidere!");
	else {
		for (int i = 0; i < dim; i++) {
			vector[i].serie = (char*)malloc(sizeof(char) * 100);
			fscanf(ft, " %[^\n]", vector[i].serie);//fgets(vector[i].serie, 100, ft);
			
			fscanf(ft, "%f", &vector[i].pret);
			fscanf(ft, "%d", &vector[i].nrPorturi);
		}
	}
	fclose(ft);
}

Calculator* citireTXT2(const char* numeFisier, int *dim) {
	(*dim) = 0;
	FILE* f = fopen(numeFisier, "r");
	Calculator* vector = NULL;
	if (!f) printf("Eroare la deschidere!");
	else {
		char buffer[100];
		char delimitator[] = ",\n";
		while (fgets(buffer, 100, f)) {
			Calculator c;
			char* token = strtok(buffer, delimitator);
			c.serie = malloc(strlen(token) + 1);
			strcpy(c.serie, token);
			token = strtok(NULL, delimitator);
			c.pret = atof(token);
			token = strtok(NULL, delimitator);
			c.nrPorturi = atoi(token);

			vector = adaugaCalculatorInVectorFaraDimensiune(c, vector, dim);
		}
		fclose(f);
	}
	return vector;
}

//MATRICE-----------------------------------------

void citireTXTmatrice(Calculator** matrice, int nrClustere, int* nrColoane) {

	FILE* f = fopen("fisier.txt", "r");
	if (!f) printf("Eroare la deschidere!");
	else {
		char buffer[100];
		char delimitator[] = ",\n";

		while (fgets(buffer, 100, f)) {
			Calculator c;
			char* token = strtok(buffer, delimitator);
			
			c.serie = (char*)malloc(sizeof(char) * (strlen(token) + 1));
			strcpy(c.serie, token);

			token = strtok(NULL, delimitator);
			c.pret = atof(token);
			token = strtok(NULL, delimitator);
			c.nrPorturi = atoi(token);

			int poz = c.nrPorturi % 2;
			matrice[poz] = adaugaCalculatorInVectorFaraDimensiune(c, matrice[poz], &(nrColoane[poz]));
		}
		fclose(f);
	}
}

void afisareMatrice(Calculator** matrice, int* nrColoane, int nrClustere) {
	for (int i = 0; i < nrClustere; i++) {
		afisareVector(matrice[i], nrColoane[i]);
	}
}

Calculator** copiereMatrice(Calculator** matrice1, int* nrColoane1, int* nrColoane2, int nrClustere) {
	for (int i = 0; i < nrClustere; i++) {
		for (int j = 0; j < nrColoane1[i]; j++) {
			if (matrice1[i][j].nrPorturi > 1)
				nrColoane2[i]++;
		}
	}

	Calculator** copie = (Calculator**)malloc(sizeof(Calculator*) * nrClustere);

	int k = 0;
	for (int i = 0; i < nrClustere; i++) {
		copie[i] = (Calculator*)malloc(sizeof(Calculator) * nrColoane2[i]);
		for (int j = 0; j < nrColoane1[i]; j++) {
			if (matrice1[i][j].nrPorturi > 1) {
				copie[i][k] = initializare(matrice1[i][j].serie, matrice1[i][j].pret, matrice1[i][j].nrPorturi);
				k++;
			}
		}
		k = 0;
	}

	return copie;
}

Calculator** copiereVectorInMatrice(Calculator* vector, int* nrColoane, int nrClustere, int dimVector) {
	for (int i = 0; i < dimVector; i++) {
		if (vector[i].nrPorturi % 2 == 0) {
			nrColoane[0]++;
		}
		else nrColoane[1]++;
	}

	Calculator** copie = (Calculator**)malloc(sizeof(Calculator*) * nrClustere);

	for (int i = 0; i < nrClustere; i++) {
		copie[i] = (Calculator*)malloc(sizeof(Calculator) * nrColoane[i]);
	}

	int k1 = 0; int k2=0;
	for (int i = 0; i < dimVector; i++) {
		if (vector[i].nrPorturi % 2 == 0) {
			copie[0][k1++] = initializare(vector[i].serie, vector[i].pret, vector[i].nrPorturi);
		} else copie[1][k2++] = initializare(vector[i].serie, vector[i].pret, vector[i].nrPorturi);
	}

	return copie;
}

Calculator* cautareDupaPret(Calculator** matrice, int* nrColoane, int nrClustere,int pret,int* dim) {
	for (int i = 0; i < nrClustere; i++) {
		for (int j = 0; j < nrColoane[i]; j++) {
			if (matrice[i][j].pret < pret) {
				(*dim)++;
			}
		}
	}
	int k = 0;
	Calculator* vector = (Calculator*)malloc(sizeof(Calculator) * (*dim));
	for (int i = 0; i < nrClustere; i++) {
		for (int j = 0; j < nrColoane[i]; j++) {
			if (matrice[i][j].pret < pret) {
				if(k<(*dim))
					vector[k++] = initializare(matrice[i][j].serie, matrice[i][j].pret, matrice[i][j].nrPorturi);
			}
		}
	}

	return vector;
}

void dezalocareMatrice(Calculator*** matrice, int** nrColoane, int* nrClustere) {
	for (int i = 0; i < (*nrClustere); i++) {
		for (int j = 0; j < (*nrColoane)[i]; j++) {
			free((*matrice)[i][j].serie);
		}
		free((*matrice)[i]);
		(*matrice)[i] = NULL;
		(*nrColoane)[i] = 0;
	}
	free(*matrice);
	*matrice = NULL;
	free(*nrColoane);
	*nrColoane = NULL;
	*nrClustere = 0;
}

void main() {
	Calculator c1 = initializare("XP 59", 39.236, 2);
	Calculator c2 = initializare("SP 1", 3020.53, 5);
	Calculator c3 = initializare("ABC", 543.33, 7);
	Calculator c4 = initializare("YO 99", 995.44, 1);
	//afisare(c1);
	//Calculator c2; citire(&c2); afisare(c2);
	int nrCalculatoare = 4;
	Calculator* c_vector = (Calculator*)malloc(sizeof(Calculator) * nrCalculatoare);
	//citire(&c_vector[0]); afisare(c_vector[0]);
	printf("\n=================VECTOR 1===============\n");
	adaugaCalculatorInVector(c1, &c_vector[0]);
	adaugaCalculatorInVector(c2, &c_vector[1]);
	adaugaCalculatorInVector(c3, &c_vector[2]);
	adaugaCalculatorInVector(c4, &c_vector[3]);
	afisareVector(c_vector,nrCalculatoare);
	printf("\n=================VECTOR 2===============\n");
	Calculator* vector2 = NULL;
	int nrCalculatoare2 = nrCalculatoare;
	vector2 = copieVector(c_vector, nrCalculatoare2);
	afisareVector(vector2, nrCalculatoare2);
	printf("\n=================VECTOR 3===============\n");
	Calculator* vector3 = NULL;
	int nrCalculatoare3 = 0;
	vector3 = adaugaCalculatorInVectorFaraDimensiune(c4, vector3 ,&nrCalculatoare3);
	vector3 = adaugaCalculatorInVectorFaraDimensiune(c3, vector3 ,&nrCalculatoare3);
	vector3 = adaugaCalculatorInVectorFaraDimensiune(c2, vector3, &nrCalculatoare3);
	vector3 = adaugaCalculatorInVectorFaraDimensiune(c1, vector3, &nrCalculatoare3);
	afisareVector(vector3,nrCalculatoare3);
	printf("Dim vect 3: %d\n", nrCalculatoare3);

	printf("\n=================VECTOR 4===============\n");
	int nr4 = 0;
	Calculator* vector4 = concatenare(vector2, vector3, nrCalculatoare2, nrCalculatoare3, &nr4);
	afisareVector(vector4,nr4);


	//FISIERE-------------------------------------------------
	printf("\n=================FISIERE===============\n");
	int dimFisier = 0;
	/*FILE* ft = fopen("fisier2.txt", "w");
	adaugareTXT(c1, ft, &dimFisier);
	adaugareTXT(c2, ft, &dimFisier);
	adaugareTXT(c3, ft, &dimFisier);
	adaugareTXT(c4, ft, &dimFisier);
	fclose(ft);
	afisareTXT("fisier2.txt");*/
	dimFisier = 4;
	Calculator* vectorFisier1 = (Calculator*)malloc(sizeof(Calculator) * dimFisier);
	citireTXT("fisier2.txt", dimFisier, vectorFisier1);
	printf("\n=================VECTOR FISIER 1===============\n");
	afisareVector(vectorFisier1, dimFisier);
	printf("\n=================VECTOR FISIER 2===============\n");
	Calculator* vectorFisier2 = NULL;
	int dim = 0;
	vectorFisier2 = citireTXT2("fisier.txt", &dim);
	afisareVector(vectorFisier2, dim);

	printf("\n=================TEST===============\n");
	int nrCalculatoareTEST = 4;
	Calculator* c_vectorTEST = (Calculator*)malloc(sizeof(Calculator) * nrCalculatoareTEST);
	copieVectorTEST(c_vector, nrCalculatoareTEST, c_vectorTEST);
	afisareVector(c_vectorTEST, nrCalculatoareTEST);


	printf("\n=================MATRICE===============\n");
	//MATRICE-----------------------------------------
	int nrClustere = 2;
	int* nrColoane = (int*)malloc(sizeof(int) * nrClustere);
	Calculator** matrice = (Calculator**)malloc(sizeof(Calculator*) * nrClustere);

	for (int i = 0; i < nrClustere; i++) {
		nrColoane[i] = 0;
		matrice[i] = NULL;
	}
	citireTXTmatrice(matrice, nrClustere, nrColoane);
	afisareMatrice(matrice, nrColoane, nrClustere);

	printf("\n=================MATRICE2===============\n");
	int nrClustere2 = 2;
	int* nrColoane2 = (int*)malloc(sizeof(int) * nrClustere2);
	nrColoane2[0] = 0; nrColoane2[1] = 0;
	Calculator** matrice2 = copiereMatrice(matrice, nrColoane, nrColoane2, nrClustere2);
	afisareMatrice(matrice2,nrColoane2, nrClustere2);

	printf("\n=================MATRICE3===============\n");
	int nrClustere3 = 2;
	int* nrColoane3 = (int*)malloc(sizeof(int) * nrClustere3);
	nrColoane3[0] = 0; nrColoane3[1] = 0;
	Calculator** matrice3 = copiereVectorInMatrice(vector2, nrColoane3, nrClustere3, nrCalculatoare2);
	afisareMatrice(matrice3,nrColoane3,nrClustere3);
	
	printf("\n=================AFISARE DUPA PRET===============\n");
	int dimpret=0;
	Calculator* vectorPret = cautareDupaPret(matrice3, nrColoane3, nrClustere3, 1000, &dimpret);
	afisareVector(vectorPret, dimpret);

	//DEZALOCARE:
	dezalocare(&c_vector, &nrCalculatoare);
	dezalocare(&vector2, &nrCalculatoare2);
	dezalocare(&vector3, &nrCalculatoare3);
	dezalocare(&vector4, &nr4);
	dezalocare(&vectorFisier1, &dimFisier);
	dezalocare(&c_vectorTEST, &nrCalculatoareTEST);
	dezalocareMatrice(&matrice, &nrColoane, &nrClustere);
	dezalocareMatrice(&matrice2, &nrColoane2, &nrClustere2);
	dezalocareMatrice(&matrice3, &nrColoane3, &nrClustere3);
	dezalocare(&vectorPret, &dimpret);
}