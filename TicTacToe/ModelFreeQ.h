#ifndef __MODELFREEQ_H__
#define __MODELFREEQ_H__

#include "Action.h"
#include "ModelFreeState.h"
#include "Q.h"
#include <unordered_map>

struct pairHash{
	public:
		template<typename T, typename U>
		std::size_t operator()(const std::pair<T,U>& x) const{
			return std::hash<T>()(x.first) ^ std::hash<U>()(x.second);
		}
};

class ModelFreeQ : public Q{
	protected:
	//using Q_TYPE = std::pair<ModelFreeState,Action>;
		//compare for "equality"
	protected:
		//std::unordered_map<Q_TYPE,double,pairHash> qHat;
		//std::unordered_map<Q_TYPE,int,pairHash> visit;	
		//virtual double alpha(const Q_TYPE& q); // == 0.8 / visit(q);
	public:
		//ModelFreeQ();
		virtual void advance()=0; 
		//virtual double max(const ModelFreeState& next);
		virtual void print()=0;
};

#endif
