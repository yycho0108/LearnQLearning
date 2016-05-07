#include "Utility.h"

double sigmoid(double x){
	//return tanh(x);
	return 1.0/(1.0 + exp(-x));
}
vec sigmoid(vec& v){
	//return tanh(v);
	return 1.0/(1.0 + exp(-v));
}
vec sigmoidPrime(vec& v, bool sig){
	if(sig){
		//return 1.0 - (v%v);
		return v % (1.0-v);
	}
	else{
		vec s = sigmoid(v);
		//return 1.0 - (s%s);
		return s % (1.0-s);
	}
}

void hline(){
	std::cout << "-------------------------" << std::endl;
}

void checkPoint(std::string s){
	std::cout << "HERE : " << s <<  std::endl;
}
