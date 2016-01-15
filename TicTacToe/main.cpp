#include <iostream>
#include "TicTacToeQ.h"
#include <signal.h>
static volatile bool run = true;

void quitHandler(int){
	run = false;
}

//#define MAX_ITER 2000
int main(int argc, char* argv[]){
	signal(SIGINT, quitHandler);
	int MAX_ITER = 20;
	if(argc == 2)
		MAX_ITER = std::atoi(argv[1]);

	TicTacToeQ q;
	q.init(3,3); //board size
	for(int i=0;i<MAX_ITER && run;++i){
		std::cerr << i << std::endl; //progress report;
		q.advance();
		//q.print();
	}
	std::cerr << "COMPLETE!" << std::endl;
	q.print();
}
