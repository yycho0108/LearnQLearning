#ifndef __Q_H__
#define __Q_H__
//#include "State.h"
class Q{
	protected:
		int t; //epoch
		//virtual double alpha();
		virtual void advance()=0;
		virtual void print()=0;
		//Q();	
};

#endif
