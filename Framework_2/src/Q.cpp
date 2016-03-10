#include "Q.h"

QKey::QKey(const sa_ptr& sa)
	:_sa(sa){}
QKey::QKey(sa_ptr&& sa):_sa(std::move(sa)){}
QKey::~QKey(){}
bool QKey::operator==(const QKey& o) const{
	return _sa->operator==(*o._sa);
	//return *_sa == *o._sa; //compare content
}
size_t QKey::hash() const{return _sa->hash();}
const sa_ptr& QKey::sa() const{return _sa;}
//QVal
QVal::QVal(double w, int v):w(w),v(v){};
void QVal::print() const{
	std::cout << "(WEIGHT : " << w << ", VISITED : " << v <<')'<< std::endl;
}
//Q
double Q::GAMMA = 0.8;
Q::Q(){t=0;}
Q::~Q(){}
void Q::setGAMMA(double g){GAMMA=g;}
double Q::alpha(const QKey& k){return 1.0/++(_q.at(k).v);} //default impl
double Q::max(const s_ptr& s){
	if(s->next().size()==0)
		return 0;
	double res = -std::numeric_limits<double>::max();
	for(auto& a : s->next()){
		auto k = QKey(std::make_shared<SA>(s,a));
		if(_q.find(k) == _q.end())
			_q.insert(std::make_pair(k,QVal()));
		res = std::max(res,_q.at(k).w); //compare weight
	}
	return res;
}
void Q::advance(int n){
	//int numIter = 0;
	for(int i=0;i<n;++i){
		for(auto& q : _q){
			//std::cout << _q.size() << std::endl;;
			//std::cout << ++numIter << std::endl;
			auto& sa = q.first.sa();
			auto& vw = q.second;
			auto& next = sa->next();
			auto r = sa->reward();
			auto ALPHA = alpha(sa);
			vw.w = (1-ALPHA)*vw.w + ALPHA*(r+GAMMA*max(next));
		}
	}
}

struct K: public s_ptr{
	K(const s_ptr& s):s_ptr(s){}
	bool operator==(const K& k){return (*this)->operator==(*k);}
	size_t hash() const{return (*this)->hash();}
};

namespace std{
	template<>
	struct hash<K>{
		size_t operator()(const K& x) const{
			return x.hash();
		}
	};
}

std::unordered_map<s_ptr,std::pair<a_ptr,double>> Q::policy() const{
	std::unordered_map<K,std::pair<a_ptr,double>> pi;
	for(auto& q : _q){
		auto& sa = q.first.sa();
		auto& vw = q.second;
		auto s = K(sa->s());
		auto& a = sa->a();
		if(pi.find(s) == pi.end())
			pi.insert(std::make_pair(s,std::make_pair(a,vw.w)));
		else{
			auto& elem = pi.at(s); 
			if(elem.second < vw.w){
				elem.first = a;
				elem.second = vw.w;
			}
		}
	}
	std::unordered_map<s_ptr,std::pair<a_ptr,double>> res;
	for(auto& p : pi){
		res.insert(std::make_pair(p.first,p.second));
	}
	return res;
}
void Q::print() const{
	std::cout << "=== PRINT ===" << std::endl;
	for(auto& q : _q){
		auto& sa = q.first.sa();
		auto& vw = q.second;
		sa->print();
		vw.print();
	}
	std::cout << "___ _____ ___" << std::endl;
}
void Q::printPolicy() const{
	std::cout << "=== POLICY ===" << std::endl;
	auto pi = policy();
	for(auto& p : pi){
		p.first->print();
		p.second.first->print();
		std::cout << "WEIGHT : " << p.second.second << std::endl;
	}
	std::cout << "SIZE : " << pi.size() << std::endl;
	std::cout << "___ ______ ___" << std::endl;
}
size_t Q::size() const{return _q.size();}

