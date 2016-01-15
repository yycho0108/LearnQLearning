#ifndef __TICTACTOEQ_H__
#define __TICTACTOEQ_H__

#include "Utility.h"
#include "ModelFreeQ.h"
#include "TicTacToeState.h"

struct Q_TYPE{
	TicTacToeState s;
	TicTacToeAction a;
	TicTacToeState _next;
	bool operator==(const Q_TYPE& other) const;
	Q_TYPE(TicTacToeState,TicTacToeAction);
	size_t ID() const;
	size_t hash();
	const TicTacToeState& S() const;
	const TicTacToeAction& A() const;
	const TicTacToeState& next() const;
	//Q_TYPE(TicTacToeState&&,TicTacToeAction&&);
};

namespace std{
	template<>
	struct hash<Q_TYPE>
	{
		size_t operator()(const Q_TYPE& x) const{
			return const_cast<Q_TYPE&>(x).hash();
		}
	};
}

class TicTacToeQ : public ModelFreeQ
{
	public:

	//using Q_TYPE = std::pair<TicTacToeState,TicTacToeAction>;
		//compare for "equality"
	protected:
		std::unordered_map<Q_TYPE,double> qHat;
		std::unordered_map<Q_TYPE,int> visit;	
		//virtual double alpha(const Q_TYPE& q); // == 0.8 / visit(q);
		//using Q_TYPE = ModelFreeQ::Q_TYPE;
		virtual double alpha(const Q_TYPE& q);
		virtual double max(const TicTacToeState& s);
	public:
		virtual void advance();
		virtual void print();
		void init(int height, int width);
		size_t size() const;
};
#endif


