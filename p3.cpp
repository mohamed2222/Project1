#include <iostream>
#include <cstdlib>
#include <cmath>
 
//define Pi
using namespace std;
 
 
class Complex{
  
	public:
		Complex();
		Complex(double real, double imaginary);
		double Re();
		double Im();
		double Mod();
		double Arg();
		
		void input();
		void output()const; //const to not change value of calling object
	
	
	private:
		double real;
		double imaginary;
	
};

const Complex operator+(Complex& complex1, Complex& complex2);
Complex operator-(Complex& complex1, Complex& complex2);
bool operator ==(const Complex& complex1, const Complex& complex2);

 
int main()
{
	Complex firstNumber, secondNumber;
	
	
	firstNumber.input();
	secondNumber.input();
	
	
	Complex total = firstNumber + secondNumber;
	
	
	total.output();
	
	
	
	return 0;
}

const Complex operator+(Complex& complex1, Complex& complex2)
{
	double all_imaginary = complex1.Im() + complex2.Im();
	
	double all_real = complex1.Re() + complex2.Re();
	
	return Complex(all_real, all_imaginary);
	
}


double Complex::Re()
{
	return real;
} 

double Complex::Im()
{
	return imaginary;
}

double Complex::Mod(){
	double real_1 = pow(real,2);
	double imaginary_1 = pow(imaginary,2);
	
	double modulus = sqrt(real_1 + imaginary_1);
	return modulus;
}
 
void Complex::input(){
	double realSection,imSection;
	
	cout<<"Enter real part ";
	cin>> realSection;
	
	real = realSection; //set class variables to these
	
	cout<<"Enter Imaginary part ";
	cin>> imSection;
	
	imaginary = imSection;
	
	
}

void Complex::output()const{
	
	cout << real<< " + ";
	cout << "i"<<imaginary ;
	
	
}

/* void RLC (float resistance, float inductance, float capacitance
float frequency){
	
	capacitor_reactance = -1/(2*pi*frequency*capacitance);
	inductor_reactance = 2*pi*frequency*inductance;
	
	total_reactance = capacitor_reactance + inductor_reactance;
	
	cout<<"Total Impedance " <<resistance<<" + i" + total_reactance;
	
} */

