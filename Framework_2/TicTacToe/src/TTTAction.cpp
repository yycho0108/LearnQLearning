#include "TTTAction.h"
#include <iostream>

int TTTAction::width = 0;
int TTTAction::height = 0;

TTTAction::TTTAction(int i, int j)
	:Action(i*width+j,i*width+j){}
TTTAction::TTTAction(size_t _ID)
	:Action(_ID,_ID){}
void TTTAction::setSize(int w, int h){
	width=w,height=h;
}
bool TTTAction::operator==(const Action& a) const{ //remove if optimize
	return ID() == a.ID();
}
size_t TTTAction::hash() const{ //remove if optimize
	return Action::hash();
}
void TTTAction::print() const{
	auto i = ID() / width;
	auto j = ID() % width;
	std::cout << '(' << i << ',' << j << ')' << std::endl;
}
