#ifndef __MODELFREESTATE_H__
#define __MODELFREESTATE_H__

#include "State.h"
class ModelFreeState : public State{
	protected:
		using ID_TYPE = State::ID_TYPE;
		//std::vector<ModelFreeState> _next;
	public:
		//ModelFreeState();
		//ModelFreeState(ModelFreeState& prev, Action a);
		virtual ID_TYPE ID() const=0;	
		
		//virtual double reward()=0;
		//virtual double reward(Action a)=0;
		//virtual double reward(Action a, ModelFreeState& s);	
		
		virtual void computeNext()=0;
		
		//virtual std::vector<Action> next() const=0;
		
		//virtual ModelFreeState next(Action) const;
		//virtual bool operator==(const ModelFreeState& s) const;
};

#endif
