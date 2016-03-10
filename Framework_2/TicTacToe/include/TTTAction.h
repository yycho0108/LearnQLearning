#ifndef __TTTACTION_H__
#define __TTTACTION_H__
#include "Action.h"
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
#endif
