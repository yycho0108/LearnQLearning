#include <map>
#include "Utility.h"
#include "Action.h"
#include "State.h"
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
