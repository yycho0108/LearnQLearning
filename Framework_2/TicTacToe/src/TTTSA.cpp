#include "TTTSA.h"
#include "TTTAction.h"
#include "TTTState.h"
using Trait = TTTState::Trait;
TTTSA::TTTSA(const s_ptr& s, const a_ptr& a)
	:SA(s,a){
}
bool TTTSA::operator==(const SA& o) const{
	const Trait& on = static_cast<TTTState*>(o.s().get())->getBoard();
	const Trait& oc = static_cast<TTTState*>(o.next().get())->getBoard(); 
	Trait n = static_cast<TTTState*>(this->s().get())->getBoard();
	Trait c = static_cast<TTTState*>(this->next().get())->getBoard();
	for(int i=0;i<4;++i){
		if(n == on && c == oc)
			return true;
		n.rotate();
		c.rotate();
	}
	n.flip();
	c.flip();
	for(int i=0;i<4;++i){
		if(n == on && c == oc)
			return true;
		n.rotate();
		c.rotate();
	}
	//n.flip(); unnecessary
	//c.flip();
	return false;
}
size_t TTTSA::hash() const{
	return SA::hash();
}
void TTTSA::print() const{
	auto ttts = static_cast<TTTState*>(s().get());
	auto ttta = static_cast<TTTAction*>(a().get());
	ttts->print(*ttta);
}
