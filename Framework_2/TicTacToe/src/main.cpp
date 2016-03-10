#include <signal.h>
#include <string>
#include <iostream>
#include "TTTQ.h"

static volatile bool run = true;
void quitHandler(int){
	run = false;
}
int main(int argc, char* argv[]){
	// [progname] [MAX_ITER] [W] [H]
	signal(SIGINT, quitHandler);
	int MAX_ITER=10;
	int w=3, h=3;
	
	if(argc>=2)
		MAX_ITER=std::atoi(argv[1]);
	if(argc==4)
		w = std::atoi(argv[2]), h = std::atoi(argv[3]);

	TTTQ q;
	q.init(w,h);
	for(int i=0;i<MAX_ITER && run;++i){
		std::cerr << i << std::endl;
		q.advance(1);
	}

	std::cerr << "COMPLETE!" << std::endl;
	q.print();
	std::cout << std::endl;
	q.printPolicy();
	std::cout << "QHATSIZE : " << q.size() << std::endl;
//	std::cout << q.size() << std::endl;
}
