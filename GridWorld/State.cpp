#include "State.h"

Grid State::states;

State::State(int ID, double reward)
	:_ID(ID),myReward(reward){

}
State::State(){

}
int State::ID() const{
	return _ID;
}
void State::set(Grid_Type& s, int h, int w){
	for(int i=0;i<h;++i){
		for(int j=0;j<w;++j){
			auto state = State(toID(i,j),s[i][j]);
			states.push_back(state);
		}
	}

}
void State::set(Topology_Type& t, int h, int w){
	for(int i=0;i<h;++i){
		for(int j=0;j<w;++j){
			if(t[i][j]){
				auto self = fromID(i);
				auto other = fromID(j);
				auto a = toAction(self.first,self.second,
						other.first, other.second);
				get(i).addNext(a,j);
			}
		}
	}
}


State& State::get(int i, int j){
	return get(toID(i,j));
}
State& State::get(int ID){
	return states[ID];
}
void State::addNext(Action a, int s){
	_next.insert(std::pair<Action,int>(a,s));
}
double State::reward(){
	return myReward;
}
double State::reward(Action a){
	return get(next(a)).reward();
}
std::map<Action,int>& State::next(){
	return _next;
}
int State::next(Action a){
	return _next.at(a);
}

