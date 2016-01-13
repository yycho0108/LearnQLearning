#include <iostream>
#include <vector>
#include <map>

#include <random>
#include <functional>
#include <ctime>

#include <cassert>
#include <limits>


#define ROW (3)
#define COL (4)
#define MAX_ITER (200)
//#define VERBOSE

class State;

double GAMMA = 0.5;

typedef std::vector<State> Grid;
typedef std::vector<std::vector<double>> Grid_Type;
typedef std::vector<std::vector<bool>> Topology_Type;
/* *** UTILITY *** */
int toID(int i, int j){
	return i * COL + j;
}
std::pair<int,int> fromID(int ID){
	return std::make_pair(ID/COL, ID%COL);
}

/* *** CLASS DECLARATION *** */

/* *** ACTION *** */
enum Action{STAY, LEFT, RIGHT, UP, DOWN};
Action toAction(int srci,int srcj, int dsti, int dstj){
	//Action::STAY is unused
	if(srci == dsti){
		return dstj>srcj?Action::RIGHT : Action::LEFT;
	}else{
		return dsti>srci?Action::DOWN : Action::UP;
	}
}
std::string& toString(Action a){
	static std::vector<std::string> v({"STAY","LEFT","RIGHT","UP","DOWN"});
	return v[(int)a];
}
/* *** STATE *** */
class State{
	//STATIC
		static Grid states;
	public:
		static State& get(int i, int j);
		static State& get(int ID);
		static void set(Grid_Type& g,int h, int w);
		static void set(Topology_Type& t,int h, int w);
	//MEMBER
	private:
		int _ID; //identifier
		double myReward;
		std::map<Action,int> _next;
	public:
		State();
		State(int ID, double reward);
		int ID() const;
		void addNext(Action,int);
		double reward(); //my reward
		double reward(Action a); //reward for action
		//State& T(Action) const; //Transition
		std::map<Action,int>& next();
		int next(Action a);
};

class Q{
	private:
		int t; //epoch
		std::map<std::pair<int,Action>,double> qHat;	
	public:
		Q();
		void set(Topology_Type& Topology,int,int);
		double alpha();
		void advance();
		void init(int ID,Action a);
		double max(int next);
		void print();
};

/* *** CLASS DEFINITION *** */

Grid State::states;
// STATE
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
// Q
Q::Q(){
	t=1;
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
		assert(a != Action::STAY);
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

const double GridWorld[3][4] = {
	{.0,.0,.0,1.0},
	{.0,-1.0,.0,-1.0},
	{.0,.0,.0,.0}
};
const bool Topology[12][12] = {
	{0,1,0,0,0,0,0,0,0,0,0,0},
	{1,0,1,0,0,1,0,0,0,0,0,0},
	{0,1,0,1,0,0,1,0,0,0,0,0},
	{0,0,1,0,0,0,0,1,0,0,0,0},
	{1,0,0,0,0,0,0,0,1,0,0,0}, //cannot visit 5
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,1,0,0,0,0,1,0,0,1,0},
	{0,0,0,1,0,0,1,0,0,0,0,1},
	{0,0,0,0,1,0,0,0,0,1,0,0},
	{0,0,0,0,0,0,0,0,1,0,1,0},
	{0,0,0,0,0,0,1,0,0,1,0,1},
	{0,0,0,0,0,0,0,1,0,0,1,0}
};

int main(){
	Grid_Type gridWorld;
	for(int i=0;i<3;++i){
		gridWorld.push_back(std::vector<double>());
		auto& row = gridWorld.back();
		for(int j=0;j<4;++j){
			row.push_back(GridWorld[i][j]);
		}
	}
	State::set(gridWorld,3,4);
	
	Topology_Type topology;
	for(int i=0;i<12;++i){
		topology.push_back(std::vector<bool>());
		auto& row = topology.back();
		for(int j=0;j<12;++j){
			row.push_back(Topology[i][j]);
		}
	}
	State::set(topology,12,12);

	Q q;
	q.set(topology,12,12);
	for(int i=0;i<MAX_ITER;++i){
#ifdef VERBOSE
		q.print();
#endif
		q.advance();
	}
	q.print();
}
