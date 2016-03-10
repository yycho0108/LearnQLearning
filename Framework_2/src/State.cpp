#include "State.h"
State::State(double r,size_t h,size_t i)
	:_reward(r),_hash(h),_ID(i){}
State::~State(){}
size_t State::hash() const{return _hash;}
size_t State::ID() const{return _ID;}
double State::reward() const{return _reward;}
