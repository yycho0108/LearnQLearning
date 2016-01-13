#include "Utility.h"

double GAMMA = 0.5;
/* *** GLOBALS *** */
int toID(int i, int j){
	return i*COL +j;
}
std::pair<int,int> fromID(int ID){
	return std::make_pair(ID/COL,ID%COL);
}
