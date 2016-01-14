#include <iostream>
#include "TicTacToeQ.h"
#define MAX_ITER 2000
int main(){
	TicTacToeQ q;
	q.init(3,3); //board size
	for(int i=0;i<MAX_ITER;++i){
		//std::cout << i << std::endl;
		q.advance();
	}
	q.print();
}
