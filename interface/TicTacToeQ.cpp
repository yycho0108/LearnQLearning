#include "TicTacToeQ.h"
#include <limits>
#include <iostream>
#include <cassert>
const double GAMMA = 0.5;


/* *** Q_TYPE *** */
//using Q_TYPE = TicTacToeQ::Q_TYPE;
Q_TYPE::Q_TYPE(TicTacToeState s,TicTacToeAction a)
	:s(s),a(a),_next(s.next(a)){
}
size_t Q_TYPE::ID() const{
	return s.ID() ^ a.ID();
}
size_t Q_TYPE::hash() const{
	return next().hash();
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
	return false;
}

void TicTacToeQ::advance(){
	for(auto& qh : qHat){
		auto& state = qh.first.S();
		auto& action = qh.first.A();
		auto next = state.next(action);
		auto& util = qh.second;
		auto r = next.reward();
		util = (1 - alpha(qh.first))*util + alpha(qh.first) * (r + GAMMA*max(next));
	}
}

void TicTacToeQ::print(){
	std::cout << "--- ---" << std::endl;
	for(auto& qh : qHat){
		std::cout << 'V' << std::endl;
		qh.first.S().print();//STATE
		qh.first.A().print();//ACTION
		std::cout << '^' << std::endl;
	}
	std::cout << "STATE SIZE : " << size() << std::endl;
	std::cout << "--- ---" << std::endl;
}

double TicTacToeQ::alpha(const Q_TYPE& q){
	//int t = ++(visit[q]);
	//visit[q] = 1;
	return 0.8 / ++visit[q];
}
double TicTacToeQ::max(const TicTacToeState& s){
	if(s.next().size() == 0)
		return 0;

	auto res = -std::numeric_limits<double>::max();
	assert(s.next().size() > 0);
	for(auto a : s.next()){ //next action
		auto tmp = qHat[Q_TYPE(s,a)];
		res = res>tmp?res:tmp;
	}
	return res;
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
}
size_t TicTacToeQ::size() const{
	return qHat.size();
}
