#include "Action.h"
#include <iostream>
Action::Action(size_t i, size_t h):_ID(i),_hash(h){};
Action::~Action(){};
bool Action::operator==(const Action& a) const{return _ID==a._ID;}
size_t Action::hash() const{return _hash;}
size_t Action::ID() const{return _ID;}
void Action::print() const{std::cout<<_ID<<std::endl;}
