#ifndef __ACTION_H__
#define __ACTION_H__
#include <string>
enum Action : unsigned int;
Action toAction(int srci,int srcj,int dsti,int dstj);
std::string& toString(Action a);
#endif
