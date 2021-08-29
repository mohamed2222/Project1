// evol.c
// ELEC1201 Lab C3: Operators and Arrays
// Evolutionary Computing
// KPZ 2018, MIT License
//
// Compile with math library:
//    gcc evol.c -lm -o evol


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#define EQUATION   y = pow(x,3) - 4
#define Y_TARGET   0.0
#define EPSILON    0.0001

#define POP_SIZE     100
#define MAX_GEN    200

#define MUTATION_STRENGTH  0.1
#define RND_INIT           100



/* void printheader(void); */
float rnd(); // Random values 0.0 to 1.0
void initpop(float *pop, int size); //*pop is the first element in the pop array, *(p+1) is the second element...
void offspring(float parent, float mutst, float *pop, int size);

int main(){
	float population[POP_SIZE];
    int   gen = 0;
    float best_ifit = FLT_MAX;  // highest possible float from c library
    float best;
    float x, y;
    float ifit;   // inverse fitness
    int i;
	int j;
	float fit;

    /* printheader(); */
    srand(RND_INIT); // srand() 2 x= 1.5784 generations 3670 srand() 1 x= 1.578401 generations 1438 srand() 3 x= 1.578401 generations 2174  
   
    /* initpop(population, POP_SIZE); */

    for (j=0;j<rnd();j++){
		
		srand(j);
		initpop(population, POP_SIZE);
		gen = 0;
		best_ifit = FLT_MAX;
		
		while( best_ifit >= EPSILON && gen < MAX_GEN ){ //i.e. the animal is less fit than the required value

			for(i =1;i < POP_SIZE; i++){
				x = population[i];
				
				

				EQUATION;  // y = f(x) - apply equation on population member to find fitness level

				ifit = fabs(y - Y_TARGET); //returns a positive double number

			   
			   if (ifit<best_ifit){ //if animal is more fit than worst possible fitness level, then this becomes new 'worst' fitness level
				   best_ifit=ifit;
				   best=x; //set best fitness level as 
			   }
			 

			}
			x = best;  //
			EQUATION;  // y = f(x)

			if( best_ifit>0){
					
					fit = 1/(best_ifit);
					x = fit;
				}
				else{
					fit = -1;
				}
			

		   /*  printf("\n Generation %4d with best solution:  x= %f --> f(x)= %f\n\n",
					   gen++, best, y); */
			
			printf("%d, %f\n",gen++,x);
			//evol > myevoldat.csv

			offspring( best, MUTATION_STRENGTH, population, POP_SIZE);
		}
		//return 0;
	}
}

void offspring(float parent, float mutst, float *pop, int size){
    int i;

    pop[0]=parent;
    for (i=1; i < size; i++){
        pop[i] = parent + mutst*(2*rnd()-1);
    }
}

void initpop (float* pop, int size){ //first argument is first element in population array
	int i = 0;
	for (i < size; i++;){
		pop[i] = rnd();
		}
}



/* void printheader(){
    printf("\n\n");
    printf("###############\n");
    printf("## Evolution ##\n");
    printf("###############\n");
}
 */

// Returns a random value between 0.0 and 1.0
float rnd(){
    return rand()/(float) RAND_MAX;//the 'float' turns the RAND_MAX into float before division
	//Rand_max returns the largest number in the random range - so it is always bigger than the rand() so division result is between 0 and 1
}