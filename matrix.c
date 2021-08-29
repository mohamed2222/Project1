#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

Matrix createMatrix(const unsigned int nRows, const unsigned int nCols)
{
	Matrix mat;
	mat.rows = nRows;
	mat.cols = nCols;
	int i;
	int j;

	mat.element = (double**)malloc(mat.rows*sizeof(double*)); //assigns memory to store pointer to rows
	for (j=0; j<mat.cols; j++)
		mat.element[j] = calloc(mat.cols,sizeof(double)); //assigns memory to store elements
	
	return mat;
}

Matrix createMatrixFromFile(const char *filename)
{
	Matrix m = {0, 0, NULL};
	double d;
	unsigned int i, j, nE = 0;
	int c;
	FILE *fPtr;
	
	/* Try to open the file */
	fPtr = fopen(filename, "r");
	if (!fPtr) { fprintf(stderr, "Could not open file: %s\n", filename); exit(EXIT_FAILURE); }
	
	/* Read the matrix to find out the number of columns and rows and dynamically allocate the memory */
	while(fscanf(fPtr, "%lf", &d) != EOF) {
		m.cols++;	//incremented once detects a space
		while((c = fgetc(fPtr)) == ' ');
		if ((c == '\n') || (c == EOF)) //while loop broken once reaches end of line/file
			break;
		ungetc(c, fPtr);
	}		

	if (m.cols) {
		rewind(fPtr);  //sets pointer back to beginning of file
		while(fscanf(fPtr, "%lf", &d) != EOF) nE++; //reads number of characters in file
		m.rows = nE / m.cols; // divides number of characters by number of columns to find number of rows
		m = createMatrix(m.rows, m.cols);
	}
	
	/* Do another pass to read in the elements */
	rewind(fPtr);
	for (i=0; i<m.rows; i++)
		for (j=0; j<m.cols; j++)
			fscanf(fPtr, "%lf", &m.element[i][j]);
	fclose(fPtr);
	
	return m;
}

void destroyMatrix(Matrix mat)
{
	int i;
	for(i=0;i<mat.rows;i++)
		free(mat.element[i]); //free up elements
	free(mat.element);//free up where element addresses stored (i.e. in reverse order to malloc - free up elements before addresses)
}

void printMatrix(const Matrix mat)
{
	unsigned int i, j;
	for (i=0; i<mat.rows; i++)
		for (j=0; j<mat.cols; j++)
			printf("[%d][%d] = %f\n", i, j, mat.element[i][j]);
}
