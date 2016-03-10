#ifndef __TTTQ_H__
#define __TTTQ_H__
#include "Q.h"
#include "TTTAction.h"
#include "TTTState.h"
#include "TTTSA.h"
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
#endif
