#include "TTTQ.h"

TTTQ::TTTQ(){};
double TTTQ::alpha(const QKey& k){
	auto v = ++(_q.at(k).v);
	return 1.0 / pow(v+1,0.85);
}
double TTTQ::max(const s_ptr& s){
	//return -Q::max(s);
	if(s->next().size()==0)
		return 0;
	double res = -std::numeric_limits<double>::max();
	for(auto& a : s->next()){
		auto k = QKey(std::make_shared<TTTSA>(s,a));
		if(_q.find(k) == _q.end())
			_q.insert(std::make_pair(k,QVal()));
		res = std::max(res,_q.at(k).w); //compare weight
	}
	return -res;
	
}
void TTTQ::advance(int n){
	for(int i=0;i<n;++i){
		for(auto& q : _q){
			//std::cout << _q.size() << std::endl;;
			//std::cout << ++numIter << std::endl;
			auto& sa = q.first.sa();
			auto& vw = q.second;
			auto& next = sa->next();
			auto r = -sa->reward(); //negate opponent's gain
			auto ALPHA = alpha(q.first);
			vw.w = (1-ALPHA)*vw.w + ALPHA*(r+GAMMA*max(next));
		}
	}
}

void TTTQ::print() const{
	Q::print();
}
void TTTQ::printPolicy() const{
	std::cout << "=== POLICY ===" << std::endl;
	auto pi = policy();
	for(auto& p : pi){
		auto& s = *static_cast<TTTState*>(p.first.get());
		auto& a = *static_cast<TTTAction*>(p.second.first.get());
		auto& w = p.second.second;
		s.print(a);
		std::cout << "WEIGHT : " << w << std::endl;
		std::cout << std::endl;
		//auto& sa = q.first.sa();
		//auto& s = *static_cast<TTTState*>(sa->s().get());
		//auto& a = *static_cast<TTTAction*>(sa->a().get());
	}
	std::cout << "SIZE : " << pi.size() << std::endl;
	std::cout << "___ ______ ___" << std::endl;

	//Q::printPolicy();
}
void TTTQ::init(int w, int h){
	TTTAction::setSize(w,h);
	TTTState::setSize(w,h);
	auto s = std::make_shared<TTTState>();
	for(int i=0;i<h;++i){
		for(int j=0;j<w;++j){
			_q.insert(std::make_pair(QKey(std::make_shared<TTTSA>(s,std::make_shared<TTTAction>(i,j))),	QVal()));
		}
	}
}
