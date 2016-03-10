#include "TTTState.h"
#include <algorithm>
#include <iostream>
#include <cassert>

int TTTState::width = 0;
int TTTState::height = 0;
enum TTTState::Cell : char{EMPTY,O,X}; //0,1,2
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
	auto isCorner = [](int i, int j){return (i==0||i==height-1) && (j==0||j==width-1);};
	auto isMiddle = [](int i, int j){return (i!=0&&i!=height-1) && (j!=0&&j!=width-1);};
	//auto isSide = [&](int i, int j){return !isCorner(i,j) && !isMiddle(i,j);};
	auto& board = *this;
	size_t C = 0;
	size_t S = 0;
	size_t M = 0;
	for(int i=0;i<height;++i){
		for(int j=0;j<width;++j){
			auto id = i*width+j;
			if(isCorner(i,j)) C+=board[id];		
			else if(isMiddle(i,j)) M+=board[id];
		   	else S+=board[id];	
		}
	}
	/*size_t C = board[0] + board[width-1] + board[(height-1)*width] + board[(height*width)-1];
	C <<= 3;
	size_t S = board[1] + board[1*width] + board[2*width-1] + board[2*width+1];
	S <<= 6;
	size_t M = board[1*width + 1]; // just for now
	*/
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
	:State(0,0,0),turn(X),board(height*width,EMPTY),done(false){ //X Starts First
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
	board.rehash(); //recalc hash
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
	if(done)
		_next.clear();
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

