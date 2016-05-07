#ifndef __AGENT_H__
#define __AGENT_H__
#include <vector>
#include <deque>
#include <iostream>

class Environment;
class Action;

class Memory{

};

class Agent{

	Agent();
	virtual ~Agent();

	virtual void process(Environment& e);
	//convert to input
	virtual void learn(Memory& m);

	virtual void memorize(Environment& prev,
			Action& a,
			double r,
			Environment& next,
			std::vector<Action> va);
	
	virtual void step(Environment& e, Action& a);
	//act upon environment
	
	virtual double maxqn(Environment& e);
};
#endif
