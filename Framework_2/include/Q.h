#ifndef __Q_H__
#define __Q_H__
#include "Action.h"
#include "State.h"
#include "SA.h"
#include "Utility.h"
#include <unordered_map>
#include <iostream>

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

#endif
