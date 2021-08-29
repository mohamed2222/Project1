#include <stdio.h>
#include <stdlib.h>
#include "vector.h"





int main(){
	Vector vec;
	Vector v;
	//const char file_name[] = "file.txt";
	
	vec = createVectorFromFile("example1.vec");
	
	printVector(vec);
	destroyVector(vec);
	
	//return 0;
}
	