#include <memory>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cassert>
/* *** Utility *** */
auto f = std::bind(std::uniform_real_distribution<double>(0.0,1.0),std::default_random_engine(time(0)));

/* *** Action *** */
class Action;
using a_ptr = std::shared_ptr<Action>;
class Action{
	protected:
		size_t _ID;
		size_t _hash;
	public:
		Action(size_t i, size_t h);
		virtual ~Action();
		virtual bool operator==(const Action&) const;
		virtual size_t hash() const;
		size_t ID() const;
		virtual void print() const;
};
namespace std{
	template<>
	struct hash<Action>{
		size_t operator()(const Action& x) const{
			return x.hash();
		}
	};
}
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
/* *** SA *** */
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
/* *** QKey *** */
class QKey{
	private:
		sa_ptr _sa;
	public:
		QKey(const sa_ptr& sa);
		QKey(sa_ptr&& sa);
		~QKey();
		bool operator==(const QKey& o) const;
		virtual size_t hash() const;
		const sa_ptr& sa() const;

};
namespace std{
	template<>
	struct hash<QKey>{
		size_t operator()(const QKey& x) const{
			return x.hash();
		}
	};
}
/* *** QVal *** */
//POD
struct QVal{
	double w; //weight
	int v; //#visited
	QVal(double w=f(),int v=0);
	void print() const;
};
/* *** Q *** */
class Q{
	protected:
		static double GAMMA;
		size_t t;
		std::unordered_map<QKey,QVal> _q;
	public:
		Q();
		~Q();
		static void setGAMMA(double);
		virtual double alpha(const QKey&);
		virtual double max(const s_ptr&);
		virtual void advance(int);
		virtual std::unordered_map<s_ptr,std::pair<a_ptr,double>> policy() const;

		virtual size_t size() const;
		virtual void print() const;
		virtual void printPolicy() const;
};
/* *** DEFINITIONS *** */
//Action
Action::Action(size_t i, size_t h):_ID(i),_hash(h){};
Action::~Action(){};
bool Action::operator==(const Action& a) const{return _ID==a._ID;}
size_t Action::hash() const{return _hash;}
size_t Action::ID() const{return _ID;}
void Action::print() const{std::cout<<_ID<<std::endl;}
//State
State::State(double r,size_t h,size_t i)
	:_reward(r),_hash(h),_ID(i){}
State::~State(){}
size_t State::hash() const{return _hash;}
size_t State::ID() const{return _ID;}
double State::reward() const{return _reward;}
//SA
SA::SA(const s_ptr& s, const a_ptr& a)
	:_s(s),_a(a),_next(s->next(a)){
	_hash = (_s->hash()*37 + _next->hash())*17 + 7; //default impl
	_reward = _next->reward();
}
SA::~SA(){}
bool SA::operator==(const SA& o) const{
	std::cout << "ERROR!! " << std::endl;
	return _s==o._s && _a == o._a;//default impl
}
size_t SA::hash() const{return _hash;}
void SA::print() const{_s->print();_a->print();}
const s_ptr& SA::s() const{return _s;}
const a_ptr& SA::a() const{return _a;}
const s_ptr& SA::next() const{return _next;}
double SA::reward() const{return _reward;}
//QKey
QKey::QKey(const sa_ptr& sa)
	:_sa(sa){}
QKey::QKey(sa_ptr&& sa):_sa(std::move(sa)){}
QKey::~QKey(){}
bool QKey::operator==(const QKey& o) const{
	return _sa->operator==(*o._sa);
	//return *_sa == *o._sa; //compare content
}
size_t QKey::hash() const{return _sa->hash();}
const sa_ptr& QKey::sa() const{return _sa;}
//QVal
QVal::QVal(double w, int v):w(w),v(v){};
void QVal::print() const{
	std::cout << "(WEIGHT : " << w << ", VISITED : " << v <<')'<< std::endl;
}
//Q
double Q::GAMMA = 0.8;
Q::Q(){t=0;}
Q::~Q(){}
void Q::setGAMMA(double g){GAMMA=g;}
double Q::alpha(const QKey& k){return 1.0/++(_q.at(k).v);} //default impl
double Q::max(const s_ptr& s){
	if(s->next().size()==0)
		return 0;
	double res = -std::numeric_limits<double>::max();
	for(auto& a : s->next()){
		auto k = QKey(std::make_shared<SA>(s,a));
		if(_q.find(k) == _q.end())
			_q.insert(std::make_pair(k,QVal()));
		res = std::max(res,_q.at(k).w); //compare weight
	}
	return res;
}
void Q::advance(int n){
	//int numIter = 0;
	for(int i=0;i<n;++i){
		for(auto& q : _q){
			//std::cout << _q.size() << std::endl;;
			//std::cout << ++numIter << std::endl;
			auto& sa = q.first.sa();
			auto& vw = q.second;
			auto& next = sa->next();
			auto r = sa->reward();
			auto ALPHA = alpha(sa);
			vw.w = (1-ALPHA)*vw.w + ALPHA*(r+GAMMA*max(next));
		}
	}
}

struct K: public s_ptr{
	K(const s_ptr& s):s_ptr(s){}
	bool operator==(const K& k){return (*this)->operator==(*k);}
	size_t hash() const{return (*this)->hash();}
};

namespace std{
	template<>
	struct hash<K>{
		size_t operator()(const K& x) const{
			return x.hash();
		}
	};
}

std::unordered_map<s_ptr,std::pair<a_ptr,double>> Q::policy() const{
	std::unordered_map<K,std::pair<a_ptr,double>> pi;
	for(auto& q : _q){
		auto& sa = q.first.sa();
		auto& vw = q.second;
		auto s = K(sa->s());
		auto& a = sa->a();
		if(pi.find(s) == pi.end())
			pi.insert(std::make_pair(s,std::make_pair(a,vw.w)));
		else{
			auto& elem = pi.at(s); 
			if(elem.second < vw.w){
				elem.first = a;
				elem.second = vw.w;
			}
		}
	}
	std::unordered_map<s_ptr,std::pair<a_ptr,double>> res;
	for(auto& p : pi){
		res.insert(std::make_pair(p.first,p.second));
	}
	return res;
}
void Q::print() const{
	std::cout << "=== PRINT ===" << std::endl;
	for(auto& q : _q){
		auto& sa = q.first.sa();
		auto& vw = q.second;
		sa->print();
		vw.print();
	}
	std::cout << "___ _____ ___" << std::endl;
}
void Q::printPolicy() const{
	std::cout << "=== POLICY ===" << std::endl;
	auto pi = policy();
	for(auto& p : pi){
		p.first->print();
		p.second.first->print();
		std::cout << "WEIGHT : " << p.second.second << std::endl;
	}
	std::cout << "SIZE : " << pi.size() << std::endl;
	std::cout << "___ ______ ___" << std::endl;
}
size_t Q::size() const{return _q.size();}

/* *** TicTacToe *** */

/* *** TTTAction *** */

class TTTAction : public Action{
	private:
		static int width,height;
	public:
		TTTAction(int,int);
		TTTAction(size_t);
		static void setSize(int,int);
		virtual bool operator==(const Action&) const;
		virtual size_t hash() const;
		virtual void print() const;
};
/* *** TTTState *** */
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
/* *** TTTSA *** */
class TTTSA : public SA{
	public:
		TTTSA(const s_ptr& s, const a_ptr& a);
		virtual bool operator==(const SA& o) const;
		virtual size_t hash() const;
		virtual void print() const;
};
class TTTQ : public Q{
	public:
		TTTQ();
		virtual double alpha(const QKey&);
		virtual double max(const s_ptr&);
		virtual void advance(int);
		virtual void print() const;
		virtual void printPolicy() const;
		void init(int w, int h);
};
/* *** DEFINITION *** */

//TTTAction
//
int TTTAction::width = 0;
int TTTAction::height = 0;

TTTAction::TTTAction(int i, int j)
	:Action(i*width+j,i*width+j){}
TTTAction::TTTAction(size_t _ID)
	:Action(_ID,_ID){}
void TTTAction::setSize(int w, int h){
	width=w,height=h;
}
bool TTTAction::operator==(const Action& a) const{ //remove if optimize
	return ID() == a.ID();
}
size_t TTTAction::hash() const{ //remove if optimize
	return Action::hash();
}
void TTTAction::print() const{
	auto i = ID() / width;
	auto j = ID() % width;
	std::cout << '(' << i << ',' << j << ')' << std::endl;
}

//TTTState
int TTTState::width = 0;
int TTTState::height = 0;
enum TTTState::Cell : char{EMPTY,O,X};
using Trait = TTTState::Trait;
using Cell = TTTState::Cell;
//TTTState::Trait
Trait::Trait(size_t l, Cell val)
	:std::vector<Cell>(l,val){
	rehash();
}
Trait::Trait(const Trait& o)
	:std::vector<Cell>(o){
	rehash();
}
size_t Trait::hash() const{
	return _hash;
}
size_t Trait::ID() const{
	size_t res=0;
	for(auto& c : *this){
		res *= 3;
		res += c;
	}
	return res;	
}

bool Trait::operator==(const Trait& t){
	return ID() == t.ID();
}
void Trait::rehash(){
	auto& board = *this;
	size_t C = board[0] + board[width-1] + board[(height-1)*width] + board[(height*width)-1];
	C <<= 3;
	size_t S = board[1] + board[1*width] + board[2*width-1] + board[2*width+1];
	S <<= 6;
	size_t M = board[1*width + 1]; // just for now
	_hash = ((17*M + S)*31+C)+7;
}
void Trait::flip(){
	for(int i=0; i<height; ++i){
		std::reverse(begin()+width*i, begin()+width*(i+1));
	}
}
void Trait::rotate(){
	transpose();
	flip();
}
void Trait::transpose(){
	for(int i=0; i<height;++i){
		for(int j=0;j<i;++j){ //don't need to flip middle
			std::swap((*this)[i*width+j], (*this)[j*width+i]);
		}
	}
}
void Trait::print() const{
	auto& board = *this;
	for(int i=0; i<height; ++i){
		for(int j=0; j<width; ++j){
			switch(board[i*width+j]){
				case EMPTY:
					putchar('-');
					break;
				case O:
					putchar('O');
					break;
				case X:
					putchar('X');
					break;
			}
		}
		std::cout << std::endl;
	}
}
void Trait::print(TTTAction& a) const{
	auto& board = *this;
	for(int i=0; i<height; ++i){
		for(int j=0; j<width; ++j){
			switch(board[i*width+j]){
				case EMPTY:
					putchar((int)a.ID() == i*width+j?'*':'-');
					break;
				case O:
					putchar('O');
					break;
				case X:
					putchar('X');
					break;
			}
		}
		std::cout << std::endl;
	}
}
Cell Trait::checkH(int i) const{
	auto& board = *this;
	auto ref = board[i*width]; // j =0;
	for(int j=1;j<width;++j){
		if(board[i*width + j] != ref)
			return EMPTY;
	}
	return ref;
}
Cell Trait::checkV(int j) const{
	auto& board = *this;
	auto ref = board[j]; //i=0;
	for(int i=1;i<height;++i){
		if(board[i*width +j] !=ref)
			return EMPTY;
	}
	return ref;
}
Cell Trait::checkD(bool rise) const{
	auto& board = *this;
	if(rise){
		auto i = height - 1;
		auto j = 0;
		auto ref = board[i*width+j]; //bottom left
		while(j < width){
			if(board[i*width+j] != ref)
				return EMPTY;
			++j, --i;
		}
		return ref;
	}else{
		auto i = 0, j = 0;
		auto ref = board[i*width+j];
		while(j < width){
			if(board[i*width+j] != ref)
				return EMPTY;
			++j, ++i;
		}
		return ref;
	}
	return EMPTY;
}
Cell Trait::check() const{
	auto& board = *this;
	for(int i=0;i<height;++i){
		auto tmp = board.checkH(i);
		if(tmp != EMPTY){
			return tmp;
		}
	}
	for(int j=0;j<width;++j){
		auto tmp = board.checkV(j);
		if(tmp != EMPTY){
			return tmp;
		}
	}
	auto tmp = board.checkD(true);
	if(tmp != EMPTY){
		return tmp;
	}
	tmp = board.checkD(false);
	if(tmp != EMPTY){
		return tmp;
	}
	return EMPTY;
}
//TTTState
TTTState::TTTState()
	:State(0,0,0),turn(O),board(height*width,EMPTY),done(false){
	for(int i=0;i<height;++i){
		for(int j=0;j<width;++j){
			_next.push_back(std::make_shared<TTTAction>(i,j));
		}
	}
	//State(reward,hash,ID)
}
TTTState::TTTState(const TTTState& t)
	:State(t),_next(t._next),turn(t.turn),board(t.board),done(t.done){
		//its fine for actions to be duplicated
		//but board needs to be free from contamination....
	//perhaps it is necessary to provide all 3 args for State initializer?
	//meh.. I gues not
}

TTTState::TTTState(const TTTState& prev,const a_ptr& a)
:TTTState(prev){
	//assert(board[a->ID()] == EMPTY);
	board[a->ID()] = turn;
	turn = (turn==O)?X:O;
	computeReward();
	auto aFind = [](const std::vector<a_ptr>& n, const a_ptr& a){
		for(auto i = n.begin(); i != n.end(); ++i){
			if(**i==*a)
				return i;
		}
		return n.end();
	};
	//assert(aFind(prev._next,a) != prev._next.end());
	//assert(aFind(_next,a) != _next.end());
	_next.erase(aFind(_next,a));
	if(_reward != 0.0 || _next.size() == 0)
		done = true;
	_ID = board.ID();
	_hash = board.hash();
}
TTTState::~TTTState(){}
void TTTState::setSize(int w, int h){
	width=w,height=h;
}
bool TTTState::operator==(const State& s) const{
	return board == static_cast<const TTTState&>(s).board;
}

size_t TTTState::hash() const{
	return State::hash();
}
size_t TTTState::ID() const{
	return State::ID();
}
s_ptr TTTState::next(const a_ptr& a){
	assert(board[a->ID()] == EMPTY);
	return std::make_shared<TTTState>(*this,a);
}
std::vector<a_ptr> TTTState::next(){
	return _next;
}
const Trait& TTTState::getBoard() const{
	return board;
}
double TTTState::reward() const{
	return State::reward();
}
double TTTState::reward(a_ptr& a){
	return -TTTState(*this,a).reward();
}
void TTTState::flip(){board.flip();}
void TTTState::rotate(){board.rotate();}
void TTTState::print() const{board.print();}
void TTTState::print(TTTAction& a) const{board.print(a);}
double TTTState::computeReward(){
	auto winner = board.check();
	if(winner != EMPTY){
		done = true;
		_reward = (turn==winner)?1.0:-1.0;
	}else{
		done=false;
		_reward=0.0;
	}
	return _reward;
}
/* *** TTTSA *** */
TTTSA::TTTSA(const s_ptr& s, const a_ptr& a)
	:SA(s,a){
}
bool TTTSA::operator==(const SA& o) const{
	const Trait& on = static_cast<TTTState*>(o.s().get())->getBoard();
	const Trait& oc = static_cast<TTTState*>(o.next().get())->getBoard(); 
	Trait n = static_cast<TTTState*>(this->s().get())->getBoard();
	Trait c = static_cast<TTTState*>(this->next().get())->getBoard();
	for(int i=0;i<4;++i){
		if(n == on && c == oc)
			return true;
		n.rotate();
		c.rotate();
	}
	n.flip();
	c.flip();
	for(int i=0;i<4;++i){
		if(n == on && c == oc)
			return true;
		n.rotate();
		c.rotate();
	}
	//n.flip(); unnecessary
	//c.flip();
	return false;
}
size_t TTTSA::hash() const{
	return SA::hash();
}
void TTTSA::print() const{
	auto ttts = static_cast<TTTState*>(s().get());
	auto ttta = static_cast<TTTAction*>(a().get());
	ttts->print(*ttta);
}
/* *** TTTQ *** */
TTTQ::TTTQ(){};
double TTTQ::alpha(const QKey& k){
	auto v = ++(_q.at(k).v);
	return 1.0 / pow(v+1,0.85);
}
double TTTQ::max(const s_ptr& s){
	//return -Q::max(s);
	if(s->next().size()==0)
		return 0;
	double res = -std::numeric_limits<double>::max();
	for(auto& a : s->next()){
		auto k = QKey(std::make_shared<TTTSA>(s,a)); ///TTTSA. /**************************
		if(_q.find(k) == _q.end())
			_q.insert(std::make_pair(k,QVal()));
		res = std::max(res,_q.at(k).w); //compare weight
	}
	return -res;
	
}
void TTTQ::advance(int n){
	for(int i=0;i<n;++i){
		for(auto& q : _q){
			//std::cout << _q.size() << std::endl;;
			//std::cout << ++numIter << std::endl;
			auto& sa = q.first.sa();
			auto& vw = q.second;
			auto& next = sa->next();
			auto r = -sa->reward(); //negate opponent's gain
			auto ALPHA = alpha(q.first);
			vw.w = (1-ALPHA)*vw.w + ALPHA*(r+GAMMA*max(next));
		}
	}
}

void TTTQ::print() const{
	Q::print();
}
void TTTQ::printPolicy() const{
	Q::printPolicy();
}
void TTTQ::init(int w, int h){
	TTTAction::setSize(w,h);
	TTTState::setSize(w,h);
	auto s = std::make_shared<TTTState>();
	for(int i=0;i<h;++i){
		for(int j=0;j<w;++j){
			_q.insert(std::make_pair(QKey(std::make_shared<TTTSA>(s,std::make_shared<TTTAction>(i,j))),	QVal()));
		}
	}
}

#include <signal.h>
static volatile bool run = true;
void quitHandler(int){
	run = false;
}
int main(int argc, char* argv[]){
	signal(SIGINT, quitHandler);
	int MAX_ITER=10;
	if(argc==2)
		MAX_ITER=std::atoi(argv[1]);
	TTTQ q;
	q.init(3,3);
	for(int i=0;i<MAX_ITER && run;++i){
		std::cerr << i << std::endl;
		q.advance(1);
	}
	std::cerr << "COMPLETE!" << std::endl;
	q.print();
	std::cout << std::endl;
	q.printPolicy();
	std::cout << "QHATSIZE : " << q.size() << std::endl;
//	std::cout << q.size() << std::endl;
}
