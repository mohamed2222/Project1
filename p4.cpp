#include <iostream>
#include <cstdlib>
#include <cmath>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <complex>
using namespace std;

int main(){
	vector<int>random;
	//creates a vector of integers
	std::default_random_engine generator;
	//'generator' is name of random engine generator
	std::uniform_int_distribution<int>distribution(1,10);
	//distribution of generated numbers is specified
	
	for (int i = 0; i<10;i++){
	    int random_number = distribution(generator);
	    //generates number between 1 and 10
		random.push_back(random_number);
		//inserts into vector
	}
	
	for(int i = 0; i<10; i++)
	    cout<<random[i]<<endl;
	//outputs vector
	cout<<"/n";
	
	vector<vector<double>>matrix(10, vector<double> (10, 0));
	//creates 2 dimensional vector
	
	for(int i = 0; i<10; i++){
	    for(int j = 0; j<10; j++){
	        double rand_double = distribution(generator);
	        matrix[i][j] = rand_double;
	    }
	}
	//fills vector with random numbers
	
	for(int i = 0; i<10; i++){
	    for(int j = 0; j<10; j++){
	        cout<<matrix[i][j]<<endl;
	    }
	}
	//outputs 2-dimensional vector
	
	
	
}
/* 
void TridiagnolSolve(double E,vector<double>D,vector<double>R,
vector<double>L,int n){
	vector<double>c(n);
	//allocates vector memory
	std::complex<double>id(0,0);
	//declares and intialises complex number
	for(int i = 0; i<n;i++)
		c[i] = E;
	//set off-diagnol elements where
	//n = no.of.off-diagnol elements;
	c[0] /= D[0];
	//divide first off-diagnol element by diagnol
	R[0] /= D[0];
	//divide first by diagnol
	for(int i = 1; i<n;i++){
		id = D[i] - c[i-1]*E;
		c[i] /= id;
		R[i] = (R[i] - c[i] * L[i+1];
	}
} */

