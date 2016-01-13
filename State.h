#ifndef __STATE__H__
#define __STATE__H__

#include "Utility.h"
#include "Action.h"
#include <vector>
#include <map>

class State;
typedef std::vector<State> Grid;

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

#endif
