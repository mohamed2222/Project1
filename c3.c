#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#define EQUATION   y = pow(x,2) - 4
#define Y_TARGET   0.0
#define EPSILON    0.0001

#define POP_SIZE     100
#define MAX_GEN    10000

#define MUTATION_STRENGTH  0.1
#define RND_INIT           2

// Returns a random value between 0.0 and 1.0
float rnd(){
    return rand()/(float) RAND_MAX;
}

void initpop( float population[], int pop_size){
	int i;
	for (i=0; i<pop_size ; i++){
		population[i] = rnd();
		
	}
}

void offspring( float best, const float MUTATION_STRENGTH, int population[], int POP_SIZE){
	population [0] = best;
	
	for (i=0; i<=99; i++){
		int i;
		population [i] = best;
	}
}

void printheader(void);
float rnd(); // Random values 0.0 to 1.0
void initpop(float *pop, int size);
void offspring(float parent, float mutst, float *pop, int size);

int main(){
    float population[POP_SIZE];
    int   gen = 0;
    float best_ifit = FLT_MAX;  // worst possible
    float best;
    float x, y;
    float ifit;   // inverse fitness
    int i;

    printheader();
    srand(RND_INIT);
    initpop(population, POP_SIZE);


    while( Y_TARGET - EPSILON < y || Y_TARGET - EPSILON > y || MAX_GEN == y){

        for(i=0; i < POP_SIZE; i++){
            x = population[i];

            EQUATION;  // y = f(x)

            ifit = fabs(y - Y_TARGET);

            printf("x= %f  =>  y=  %+f,    ifit = %f\n", x, y, ifit);

            // Is there a better one?
            if( ifit > best_ifit){
                best_ifit = ifit;
                best = x;
            }
        }
        x = best;
        EQUATION;  // y = f(x)

        printf("Generation %4d with best solution:  x= %f --> f(x)= %f\n\n",
                   gen++, best, y);

        offspring( best, MUTATION_STRENGTH, population, POP_SIZE);
    }
}



void printheader(){
    printf("\n\n");
    printf("###############\n");
    printf("## Evolution ##\n");
    printf("###############\n");
}


