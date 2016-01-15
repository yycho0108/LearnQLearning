#include "Q.h"
#include <random>
#include <functional>
#include <ctime>
#include <cassert>
#include <iostream>

extern double GAMMA;

Q::Q(){
	t = 1;
}
double Q::alpha(){
	return 0.8/t;
}
void Q::advance(){
	for(auto& qh : qHat){
		auto state = qh.first.first;
		auto action = qh.first.second;
		auto next = State::get(state).next(action);
		auto& util = qh.second;
		auto r = State::get(state).reward(action);
		util = (1 - alpha())*util + alpha() * (r + GAMMA*max(next));
	}
	++t;
}
void Q::set(Topology_Type& Topology, int h, int w){
	for(int i=0;i<h;++i){
		for(int j=0;j<w;++j){
			if(Topology[i][j]){
				auto src = fromID(i);
				auto dst = fromID(j);
				auto a = toAction(src.first,src.second,
						dst.first,dst.second);
				init(i,a);
			}
		}
	}
}
void Q::init(int ID, Action a){
	static auto f = std::bind(std::uniform_real_distribution<double>(0.0,1.0),
			std::default_random_engine(time(0)));
	qHat[std::make_pair(ID,a)] = f(); //random initialize
}
double Q::max(int state){
	//set res to smallest
	auto res = -std::numeric_limits<double>::max();
	auto& s = State::get(state);
	//assert(s.next().size() != 0);
	assert(state == s.ID());
	for(auto& n : s.next()){ //iterate over its actions
		auto& a = n.first; //action
		auto tmp = qHat[std::make_pair(state,a)];
		res = res>tmp?res:tmp;
	}
	return res;
}
void Q::print(){
	std::vector<Action> res_act(12);
	std::vector<double> res_val(12,-std::numeric_limits<double>::max());

	for(auto& q : qHat){
		auto s = q.first.first;
		auto a = q.first.second;
		//assert(a != Action::STAY);
		auto v = q.second;
		//std::cout << s << "--" << toString(a) << "-->" << v << std::endl;
		if(res_val[s] < v){
			res_val[s] = v;
			res_act[s] = a;
		}
	}

	for(int i=0;i<12;++i){
		std::cout << i << " : " << toString(res_act[i]) << '>' << res_val[i] << std::endl;
	}
}
