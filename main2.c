#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"




int main(){
	Matrix mat;
	Matrix m;
	//const char file_name[] = "file.txt";
	
	mat = createMatrixFromFile("example1.mat");
	
	printMatrix(mat);
	destroyMatrix(mat);
	
	//return 0;
}