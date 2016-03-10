#ifndef __SA_H__
#define __SA_H__
#include "Action.h"
#include "State.h"
#include <memory>
#include <iostream>

class SA;
using sa_ptr = std::shared_ptr<SA>;
class SA{
	private:
		s_ptr _s;
		a_ptr _a;
		s_ptr _next;
		size_t _hash;
		double _reward;
	public:
		SA(const s_ptr& s, const a_ptr& a);
		virtual ~SA();
		virtual bool operator==(const SA& o) const;
		virtual size_t hash() const;
		virtual void print() const;
		//getters
		const s_ptr& s() const;
		const a_ptr& a() const;
		const s_ptr& next() const;
		double reward() const;
}; //may be overridden to alter operator==
namespace std{
	template<>
	struct hash<SA>{
		size_t operator()(const State& x) const{
			return x.hash();
		}
	};
}
#endif
