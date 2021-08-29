#ifndef _MATRIX_H
#define _MATRIX_H

/* Data structure */ 

typedef struct {
	int rows;
	int cols;
	double **element; //must declare a pointer when do not know the size of array
	
} Matrix;

/* Interfaces */

Matrix createMatrix(const unsigned int nRows, const unsigned int nCols);
Matrix createMatrixFromFile(const char *filename);
void destroyMatrix(Matrix mat);
void printMatrix(const Matrix mat);

#endif