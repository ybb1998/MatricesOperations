#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

double* randomVector(int length);
int epsilonCheck(double* vecA, double* vecB, int length);
void nextVectorCalc(double* dotProduct, double norm, int length);
void maxEigenCalc(FILE* inputFile,	FILE* outputFile, int length);

double* randomVector(int length){
	int i;
	double *vector;
	srand(time(NULL));

	vector = (double*) malloc(length * sizeof(double));

	for (i = 0 ; i < length ; i++){
		vector[i] = (double) rand();
	}
	return vector;
}

int epsilonCheck(double* vecA, double* vecB, int length){
	int i;

	for( i = 0 ; i < length ; i++){
		if (fabs( (double) (vecA[i] - vecB[i])) > 0.00001){
			return 0;
		}
	}

	return 1;
}

void nextVectorCalc(double *dotProduct, double norm, int length){
	int i;

	for (i = 0 ; i < length ; i++){
		dotProduct[i] = (dotProduct[i] / norm);
	}
}

void maxEigenCalc(FILE* inputFile, FILE* outputFile, int length){
	double sum = 0, norm = 0;
	int i, j, difference = 0, size = 0;
	double *currVec, *temp, *nextVec, *matRow;

	nextVec = (double*) malloc(length * sizeof(double));
	matRow = (double*) malloc(length * sizeof(double));
	currVec = randomVector(length);

	while (difference == 0){

		fseek(inputFile, 2 * sizeof(int), SEEK_SET);

		for (i = 0; i < length ; i++){
			size = fread(matRow, sizeof(double), length, inputFile);
			assert (size == length);

			for ( j = 0 ; j < length ; j++){
				sum += (matRow[j] * currVec[j]);
			}

			nextVec[i] = sum;
			norm += sum * sum;
			sum = 0;
		}

		nextVectorCalc(nextVec, sqrt(norm), length);

		norm = 0;

		difference = epsilonCheck(nextVec, currVec, length);

		temp = currVec;
		currVec = nextVec;
		nextVec = temp;

		rewind(inputFile);
	}

	fwrite(currVec, sizeof(double), length, outputFile);

	free(temp);
	free(currVec);
	free(matRow);
}

int main(int argc, char* argv[]){
	FILE* inputFile, *outputFile;
	int length;
	const int one = 1;
	clock_t sTime, eTime;

	(void)argc;
	sTime = clock();

	inputFile = fopen(argv[1], "r");
	assert(inputFile!=NULL);
	fread(&length, sizeof(int), 1, inputFile);
	rewind(inputFile);

	outputFile = fopen(argv[2], "w");
	fwrite(&one, sizeof(int), 1, outputFile);
	fwrite(&length, sizeof(int), 1, outputFile);

	maxEigenCalc(inputFile, outputFile, length);

	fclose(inputFile);
	fclose(outputFile);

	eTime = clock();

	return 1;
}

