#ifndef __TICTACTOESTATE_H__
#define __TICTACTOESTATE_H__

#include "Utility.h"
#include "Action.h"
#include "ModelFreeState.h"

class TicTacToeAction : public Action{
	using type = Action::type;
	/* *** STATIC *** */
	private:
		static int width,height;
	public:
		static void setSize(int w, int h);
	/* *** MEMBER *** */
	public:
		TicTacToeAction(int i,int j);
		bool operator==(const TicTacToeAction& a) const;
		virtual type ID() const;
		virtual void print() const;
		//bool operator<(Action& other);
};

//enum Action{TL,TM,TR,ML,MM,MR,BL,BM,BR};

class TicTacToeState : public ModelFreeState{
	public:
		enum Cell:char;
		using ID_TYPE = ModelFreeState::ID_TYPE;
		struct Trait : public std::vector<Cell>{
			bool operator==(const Trait& t);
			ID_TYPE ID() const;
			ID_TYPE hash();
			ID_TYPE _hash = 0;
			void initHash();
			void flip();
			void rotate();
			void transpose();
			void print() const;
			void print(TicTacToeAction& a) const;
			Cell checkHorz(int i) const;
			Cell checkVert(int j) const;
			Cell checkDiag(bool rise) const;
		};
		//using Trait = std::vector<Cell>;
	/* *** STATIC *** */
		static int width,height;
		static void setSize(int w, int h);
	/* *** MEMBER *** */
	private:
		Cell turn;
		Trait board; //Trait = unique identifier
		std::vector<TicTacToeAction> _next;
		int _empty; //Convenience
		bool done;
		double _reward;
	public:
		TicTacToeState();
		TicTacToeState(const TicTacToeState& prev, TicTacToeAction a);
		TicTacToeState(const TicTacToeState& t);
		/*
		bool isCorner(int i, int j) const;
		bool isSide(int i, int j) const;
		bool isMiddle(int i, int j) const;
		*/
		virtual ID_TYPE ID() const;
		virtual ID_TYPE hash();
		virtual double reward();
		virtual double reward(TicTacToeAction a);
		//virtual double reward(Action a, TicTacToeState& s);
		virtual void computeReward();
		virtual void computeNext();
		virtual std::vector<TicTacToeAction> next() const;
		virtual TicTacToeState next(TicTacToeAction) const;
		virtual bool operator==(const TicTacToeState& s) const;
		//Utility
		
		int empty() const;
		void randomMove(Cell t);
		void print() const;
		void print(TicTacToeAction& a) const;	
		void flip();
		void rotate();
};




namespace std{
	template<>
	struct hash<TicTacToeState>
	{
		size_t operator()(const TicTacToeState& x) const{
			return x.ID();
		}
	};
}


namespace std{
	template<>
	struct hash<TicTacToeAction>
	{
		size_t operator()(const TicTacToeAction& x) const{
			return x.ID();
		}
	};
}

#endif
