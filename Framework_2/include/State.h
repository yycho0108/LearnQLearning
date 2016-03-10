#ifndef __STATE_H__
#define __STATE_H__
#include "Action.h"
#include <memory>
#include <vector>
class State;
using s_ptr = std::shared_ptr<State>;
/* *** State  *** */
class State{
	protected:
		double _reward;
		size_t _hash;
		size_t _ID;
		//std::vector<a_ptr> _next;
	public:
		State(double r,size_t h,size_t i);
		virtual ~State();
		//State(State&,Action);

		//comparator
		virtual bool operator==(const State&) const=0;
		virtual size_t hash() const;
		virtual size_t ID() const;
		
		//T,R
		virtual s_ptr next(const a_ptr& a)=0;// = return new State(*this,a);
		virtual std::vector<a_ptr> next()=0;// = return vector of new actions	
		virtual double reward() const;
		virtual double reward(a_ptr& a)=0;
		
		//utility
		virtual void print() const=0;
		//double max();

};
namespace std{
	template<>
	struct hash<State>{
		size_t operator()(const State& x) const{
			return x.hash();
		}
	};
}
#endif
