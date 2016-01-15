#include "TicTacToeQ.h"
#include <limits>
#include <iostream>
#include <cassert>
#include <cmath>
const double GAMMA = 0.9;

/* *** Q_TYPE *** */
//using Q_TYPE = TicTacToeQ::Q_TYPE;
Q_TYPE::Q_TYPE(TicTacToeState s,TicTacToeAction a)
	:s(s),a(a),_next(s.next(a)){
}
size_t Q_TYPE::ID() const{
	return s.ID() ^ a.ID();
}
size_t Q_TYPE::hash(){
	return const_cast<TicTacToeState&>(next()).hash();
	//return const_cast<TicTacToeState&>(s).hash() ^
	//	const_cast<TicTacToeState&>(next()).hash();
}
const TicTacToeState& Q_TYPE::S() const{
	return s;
}
const TicTacToeAction& Q_TYPE::A() const{
	return a;
}
const TicTacToeState& Q_TYPE::next() const{
	return _next;
}

bool Q_TYPE::operator==(const Q_TYPE& other) const{

	//compares equal for rot & sym
	auto& n = const_cast<TicTacToeState&>(next());
	for(int i=0;i<4;++i){
		if(n == other.next())
			return true;
		n.rotate();
	}
	n.flip();
	for(int i=0;i<4;++i){
		if(n == other.next())
			return true;
		n.rotate();
	}
	n.flip(); //now all is as it was before!
	//next().flip();
	//std::cout << "FALSE! NOT SAME!" << std::endl;
	return false;
}

void TicTacToeQ::advance(){
	for(auto& qh : qHat){
		auto& state = qh.first.S();
		auto& action = qh.first.A();
		auto next = state.next(action);
		auto& util = qh.second;
		
		//just to get reward
		auto& s = const_cast<TicTacToeState&>(state);
		auto r = s.reward(action); //immediate reward for an action
		auto ALPHA = alpha(qh.first);	
		util = (1 - ALPHA)*util + ALPHA * (r + GAMMA*max(next)); // max qHat for next
	}
}

void TicTacToeQ::print(){
	std::cout << "--- ---" << std::endl;
	std::unordered_map<TicTacToeState, std::pair<TicTacToeAction,double>> m;
	std::unordered_map<TicTacToeState,int> visit;
	for(auto& qh : qHat){
		auto& s = qh.first.S();
		++visit[s];
		auto& a = qh.first.A();
		
		if(m.find(s) == m.end())
			m.insert(std::make_pair(s,std::pair<TicTacToeAction,double>(a,-1.0)));

		auto& loc = m.at(s);
		if(loc.second < qh.second){
			loc.first = a;
			loc.second = qh.second;
		}
		s.print(const_cast<TicTacToeAction&>(a));
		auto& v = qh.second;
		std::cout << "WEIGHT : " << v << std::endl;
		//a.print();
		//s.print();
	}

	for(auto& elem : visit){
		if(elem.second != elem.first.empty()){
			std::cout << "EMPTY : " << elem.first.empty() << ',';
			std::cout << "NEXT : " << elem.first.next().size() << ',';
			std::cout << "VISITED : " << elem.second << std::endl;
			elem.first.print();
			char c;
			std::cin >> c;
		}
	}
	std::cout << "<REPORT> " << std::endl;
	for(auto& elem : m){
		auto& s = elem.first;
		auto& a = elem.second.first;
		auto& v = elem.second.second;
		std::cout << "---------------" << std::endl;
		s.print(a);
		//s.print();
		//a.print();
		std::cout << "WEIGHT : " << v << std::endl;
		std::cout << std::endl;
		std::cout << "---------------" << std::endl;
	}

	std::cout << "STATE SIZE : " << m.size() << std::endl;
	std::cout << "--- ---" << std::endl;
}

double TicTacToeQ::alpha(const Q_TYPE& q){
	//return 0.2;
	auto v = ++visit[q];
	return 1.0 / pow(v+1,0.85);
}

//try to maximize profit by "minimizing loss"
double TicTacToeQ::max(const TicTacToeState& s){
	if(s.next().size() == 0) //nothing after.
		return 0;
	auto res = 99999.0; //a very high value
	//assert(s.next().size() > 0);
	for(auto a : s.next()){ //next action
		auto tmpQ = Q_TYPE(s,a);
		if(qHat.find(tmpQ) == qHat.end()){
			qHat[tmpQ] = f();
		}
		auto tmp = qHat[tmpQ];
		res = res<tmp?res:tmp; //maximize my profit by minimizing loss
	}
	return -res; // maximum profit for s
}

void TicTacToeQ::init(int height, int width){

	TicTacToeAction::setSize(width,height);
	TicTacToeState::setSize(width,height);
	//auto t = TicTacToeState();
	for(int i = 0;i<height;++i){
		for(int j=0;j<width;++j){
			//auto a = TicTacToeAction(i,j);
			qHat[Q_TYPE(TicTacToeState(),TicTacToeAction(i,j))] = f();
		}
	}
	std::cout << qHat.size() << std::endl;
	print();
}
size_t TicTacToeQ::size() const{
	return qHat.size();
}
