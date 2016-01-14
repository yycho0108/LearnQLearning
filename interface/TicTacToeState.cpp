#include "TicTacToeState.h"

#include <functional>
#include <ctime>
#include <random>
#include <algorithm>
#include <cassert>


#include <iostream> //for debug


using ID_TYPE = TicTacToeState::ID_TYPE;

/* *** TicTacToeAction *** */
int TicTacToeAction::width;
int TicTacToeAction::height;

void TicTacToeAction::setSize(int w, int h){
	width = w;
	height = h;
}
TicTacToeAction::TicTacToeAction(int i, int j)
	:Action(i*width+j){
		//NOTHING
}

TicTacToeAction::type TicTacToeAction::ID() const{
	return _ID; // 0<= x < 9
}
bool TicTacToeAction::operator==(const TicTacToeAction& a) const{
	return _ID == a._ID;
}

void TicTacToeAction::print() const{
	std::cout << "(i:" << (_ID/width) << "j:" << (_ID%width) << ")" << std::endl;
}

/* *** TicTacToeState *** */

//Trait
using Trait = TicTacToeState::Trait;
TicTacToeState::ID_TYPE Trait::ID() const{ // unique ID
	ID_TYPE res = 0;
		for(auto& c : *this){
			res *= 3;
			res += c;
		}
	return res;	
}

TicTacToeState::ID_TYPE Trait::hash() const{
	//well.. if larger than 3*3, this will become harder...
	auto& board = *this;
	ID_TYPE C = board[0] + board[width-1] + board[(height-1)*width] + board[(height*width)-1];
	C <<= 3;
	ID_TYPE M = board[1*width + 1]; // just for now
	ID_TYPE S = board[1] + board[1*width] + board[2*width-1] + board[2*width+1];
	S <<= 6;
	//auto res = M^(C+39)^(S+91);
	//std::cout << res << std::endl;
	return M^(C+39)^(S+91);
	//return C^(M<<3)^(S<<6);
}

bool Trait::operator==(const Trait& t){
	return ID() == t.ID();
}

void Trait::transpose(){
	for(int i=0; i<height;++i){
		for(int j=0;j<=i;++j){
			std::swap((*this)[i*width+j], (*this)[j*width+i]);
		}
	}
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
//
int TicTacToeState::width;
int TicTacToeState::height;
enum TicTacToeState::Cell : char{EMPTY,O,X};
using Cell = TicTacToeState::Cell;

void TicTacToeState::setSize(int w, int h){
	width = w;
	height = h;
}

TicTacToeState::TicTacToeState()
{
	done = false;
	empty = height*width;
	for(int i=0;i<height;++i){
		for(int j=0;j<width;++j){
			board.push_back(EMPTY); //empty
		}
	}
	turn = O; //default : O Starts First
	computeNext();
	//std::cout << _next.size() << std::endl;
	

	// COMMENTED OUT FOR LEGISTICS STUFF :
	// WILL BE RESTORED.
	// BUT FOR NOW, "I" ONLY START FIRST.
	//if(f() < 0.5){
	//	randomMove(turn); // = Opponent played first
	//	turn = X;
	//	--empty;
	//}
}
TicTacToeState::TicTacToeState(const TicTacToeState& prev, TicTacToeAction a)
:board(prev.board),
_next(prev._next),
empty(prev.empty -1),
done(prev.done){
	auto id = a.ID();
	auto i = id / width;
	auto j = id % width;
	
	//CHANGE BOARD
	board[i*width+j] = prev.turn;
	auto it = std::find(_next.begin(),_next.end(),a); //where action occurs
	assert(it != _next.end());
	_next.erase(it);	
	/* *** OPPONENT MAKES A MOVE *** */
	if(empty >0){
		//turn = (prev.turn==X)?O:X;
		auto t = (prev.turn == X)?O:X;
		randomMove(t);
		--empty;
	}
	turn = prev.turn; //flip twice
	//if (_next.size() == 0)
	//   done = true;
	//computeNext();
}

TicTacToeState::TicTacToeState(const TicTacToeState& t)
:turn(t.turn),board(t.board),_next(t._next),empty(t.empty),done(t.done){

}

//TicTacToeState::TicTacToeState(TicTacToeState&& t)
//:turn(t.turn),board(t.board),_next(t._next),empty(t.empty),done(t.done){
//}

void TicTacToeState::computeNext(){
	if(!done){		
		for(int i=0; i<height;++i){
			for(int j=0;j<width;++j){
				if(board[i*width+j] == EMPTY){
					_next.push_back(TicTacToeAction(i,j));	
				}
			}
		}
	}

}
Cell TicTacToeState::checkHorz(int i){
	auto ref = board[i*width]; // j =0;
	for(int j=1;j<width;++j){
		if(board[i*width + j] != ref)
			return EMPTY;
	}
	return ref;
}
Cell TicTacToeState::checkVert(int j){
	auto ref = board[j]; //i=0;
	for(int i=1;i<height;++i){
		if(board[i*width +j] !=ref)
			return EMPTY;
	}
	return ref;
}
Cell TicTacToeState::checkDiag(bool rise){
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
double TicTacToeState::reward(){
	done = true;
	std::vector<TicTacToeAction> v;
	_next.swap(v);
	
	//CHECK
	for(int i=0;i<height;++i){
		auto tmp = checkHorz(i);
		if(tmp != EMPTY)
			return tmp==turn?1.0 : -1.0; // is my "TURN" what I win for? hmm... I guess.	
	}
	for(int j=0;j<width;++j){
		auto tmp = checkVert(j);
		if(tmp != EMPTY)
			return tmp==turn?1.0:-1.0;
	}
	auto tmp = checkDiag(true);
	if(tmp != EMPTY)
		return tmp == turn? 1.0 : -1.0;
	tmp = checkDiag(false);
	if(tmp != EMPTY)
		return tmp == turn? 1.0 : -1.0;
	
	_next.swap(v);
	done = false;
	return 0.0;
	//examine state
}

double TicTacToeState::reward(TicTacToeAction a){
	return next(a).reward();
}

std::vector<TicTacToeAction> TicTacToeState::next() const{
	return _next;
}
TicTacToeState TicTacToeState::next(TicTacToeAction a) const{
	assert(std::find(_next.begin(),_next.end(),a) != _next.end());
	return TicTacToeState(*this,a);
}

bool TicTacToeState::isCorner(int i, int j) const{
	return (i==0 && j==0)
		|| (i==0 && j+1==width)
		|| (i+1==height && j==0)
		|| (i+1==height && j+1==width);
}

bool TicTacToeState::isSide(int i, int j) const{
	return !isCorner(i,j) && !isMiddle(i,j);
}

bool TicTacToeState::isMiddle(int i, int j) const{
	return 0<i && 0<j && i+1<height && j+1<width;
}

TicTacToeState::ID_TYPE TicTacToeState::ID() const{
	return board.ID();

}
TicTacToeState::ID_TYPE TicTacToeState::hash() const{
	return board.hash();
	/*
	 *
	 * ID_TYPE C=0;
	ID_TYPE M=0;
	ID_TYPE S=0;

	for(int i=0;i<height;++i){
		for(int j=0;j<width;++j){
			if(isCorner(i,j)){
				++C;
			}else if (isMiddle(i,j)){
				++M;
			}else{ //Middle
				++S;
			}

		}
	}

	return (C<<10) + (M<<5) + S;
	*/ //HEAVY HASH
}
void TicTacToeState::randomMove(Cell t){
	assert(_next.size() >0);
	std::random_shuffle(_next.begin(),_next.end());
	auto a = _next.back();
	board[a.ID()] = t;
	_next.pop_back();
}

bool TicTacToeState::operator==(const TicTacToeState& s) const{
	return board == s.board;
}

void TicTacToeState::print() const{
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
	std::cout << std::endl;
}

void TicTacToeState::flip(){
	board.flip();
}

void TicTacToeState::rotate(){
	board.rotate();
}
