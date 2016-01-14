#include "State.h"

class ModelBasedState : public State{
	/* STATIC */
	using States = std::vector<ModelBasedState>;
	using ID_TYPE = int;
	static States states;
	public:
		static ModelBasedState& get(int i, int j);
		static ModelBasedState& get(ID_TYPE ID);
		static void set(std::vector<double>& m); //set Local Rewards
		static void set(std::vector<std::vector<bool>>& t); //set Topology
	/* MEMBER */
	protected:
		ID_TYPE _ID;
		double _reward;
	public:
		ModelBasedState();
		ModelBasedState(ID_TYPE ID, double reward);
		virtual ID_TYPE ID() const;
		virtual double reward();
		virtual double reward(Action a);
		std::map<Action,int>& next();
		ID_TYPE next(Action a);
};
