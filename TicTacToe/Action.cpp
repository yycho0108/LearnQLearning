#include "Action.h"
#include <iostream>

Action::Action(type ID){
	_ID = ID;
}
Action::type Action::ID() const{
	return _ID;
}
bool Action::operator==(const Action& a) const{
	return _ID == a._ID;
}
void Action::print() const{
	std::cout << _ID;
}
