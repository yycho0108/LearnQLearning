#ifndef __TTTSTATE_H__
#define __TTTSTATE_H__
#include "State.h"
#include "TTTAction.h"
class TTTState : public State{
public:
	enum Cell:char;
	struct Trait : public std::vector<Cell>{
		private:
			size_t _hash=0;
		public:
			Trait(size_t l, Cell val);
			Trait(const Trait&);
			
			size_t hash() const;
			size_t ID() const;
			bool operator==(const Trait&);
			void rehash();
			
			void flip();
			void rotate();
			void transpose();
			void print() const;
			void print(TTTAction&) const;
			Cell checkH(int) const;
			Cell checkV(int) const;
			Cell checkD(bool) const;
			Cell check() const;
	};
private:
	static int width,height;
	std::vector<a_ptr> _next;
	Cell turn;
	Trait board;
	bool done;
public:
	TTTState();
	TTTState(const TTTState&);
	TTTState(const TTTState&,const a_ptr&);
	~TTTState();
	static void setSize(int w, int h);
	virtual bool operator==(const State&) const;
	virtual size_t hash() const;
	virtual size_t ID() const;

	virtual s_ptr next(const a_ptr& a);
	virtual std::vector<a_ptr> next();
	virtual double reward() const;
	virtual double reward(a_ptr& a);
	const Trait& getBoard() const;
	
	void flip();
	void rotate();
	virtual void print() const;
	void print(TTTAction&) const;
	double computeReward();
};

#endif
