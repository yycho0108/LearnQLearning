#include "Action.h"
#include <vector>
enum Action : unsigned int {STAY,LEFT,RIGHT,UP,DOWN};
Action toAction(int srci,int srcj, int dsti, int dstj){
	//Action::STAY is unused
	if(srci == dsti){
		return dstj>srcj?Action::RIGHT : Action::LEFT;
	}else{
		return dsti>srci?Action::DOWN : Action::UP;
	}
}

std::string& toString(Action a){
	static std::vector<std::string> v({"STAY","LEFT","RIGHT","UP","DOWN"});
	return v[(int)a];
}
