#ifndef __STATE_H__
#define __STATE_H__

#include "Action.h"
#include <vector>
#include <map>

class State{
	public:
		//use size_t? well..
		using ID_TYPE = int;
		//State();
		//virtual ID_TYPE ID() const=0; //unique ID of state
		//virtual double reward()=0;
		//virtual State next(Action a);
		//virtual std::vector<Action> next() const=0;
};

#endif
