#include <SA.h>
SA::SA(const s_ptr& s, const a_ptr& a)
	:_s(s),_a(a),_next(s->next(a)){
	_hash = (_s->hash()*37 + _next->hash())*17 + 7; //default impl
	_reward = _next->reward();
}
SA::~SA(){}
bool SA::operator==(const SA& o) const{
	std::cout << "ERROR!! " << std::endl;
	return _s==o._s && _a == o._a;//default impl
}
size_t SA::hash() const{return _hash;}
void SA::print() const{_s->print();_a->print();}
const s_ptr& SA::s() const{return _s;}
const a_ptr& SA::a() const{return _a;}
const s_ptr& SA::next() const{return _next;}
double SA::reward() const{return _reward;}

