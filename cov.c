#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void covMatrixCreator(FILE* outputFile, double* matrix, int nRows, int nCols);
double dotProduct(double* rowA, double* rowB, int length);
void standardize(double* matrix, int nRows, int nCols);
double mean(double* row, double length);
void substract(double* row, double mean, int length);

void covMatrixCreator(FILE* outputFile, double* matrix, int nRows, int nCols){
	int i, j;
	double value;
	for (i = 0 ; i <= nRows*nCols - nCols ; i += nCols){
		for (j = 0 ; j <= nRows*nCols - nCols ; j += nCols){
			value = dotProduct(&matrix[i], &matrix[j], nCols);
			fwrite(&value, sizeof(double), 1, outputFile);
		}
	}
}

double dotProduct(double* rowA, double* rowB, int length){
	double result = 0;
	int i;
	for (i = 0 ; i < length; i++){
		result += rowA[i] * rowB[i];
	}
	return result;
}

void standardize(double* matrix, int nRows, int nCols){
	double rowMean;
	int i;
	for (i = 0 ; i <= nRows*nCols - nCols ; i += nCols){
		rowMean = mean(&matrix[i], nCols);
		substract(&matrix[i], rowMean, nCols);
	}
}

double mean(double* row, double length){
	double sum = 0, result = 0;
	int i;
	for (i = 0; i < length ; i++){
		sum += row[i];
	}
	result = sum / length;
	return result;
}

void substract(double* row, double mean, int length){
	int i;
	for (i = 0 ; i < length; i++){
		row[i] = row[i] - mean;
	}
}

int main(int argc, char* argv[]){
	FILE* inputFile;
	FILE* outputFile;
	int dim[2], nRows, nCols;
	double* inputMat;
	clock_t sTime, eTime;

	(void)argc;

	sTime = clock();
	inputFile = fopen(argv[1], "r");
	assert(inputFile != NULL);
	fread(dim, sizeof(int), 2, inputFile);
	nCols = dim[0];
	nRows = dim[1];

	inputMat = (double*) malloc(nCols * nRows * sizeof(double));
	fread(inputMat, sizeof(double), nCols * nRows, inputFile);

	standardize(inputMat, nRows, nCols);

	outputFile = fopen(argv[2], "w");
	fwrite(&nRows, sizeof(int), 1, outputFile);
	fwrite(&nRows, sizeof(int), 1, outputFile);

	covMatrixCreator(outputFile, inputMat, nRows, nCols);

	fclose(outputFile);
	fclose(inputFile);
	free(inputMat);

	eTime = clock();
	return 1;
}
