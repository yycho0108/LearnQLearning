#include <iostream>
#include "TicTacToeQ.h"
//#define MAX_ITER 2000
int main(int argc, char* argv[]){
	int MAX_ITER = 200;
	if(argc == 2)
		MAX_ITER = std::atoi(argv[1]);

	TicTacToeQ q;
	q.init(3,3); //board size
	for(int i=0;i<MAX_ITER;++i){
		std::cout << i << std::endl;
		q.advance();
		//q.print();
	}
	q.print();
}
