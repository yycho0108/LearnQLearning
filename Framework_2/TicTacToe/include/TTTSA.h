#ifndef __TTTSA_H__
#define __TTTSA_H__
#include "SA.h"
#include "TTTAction.h"
#include "TTTState.h"
class TTTSA : public SA{
	public:
		TTTSA(const s_ptr& s, const a_ptr& a);
		virtual bool operator==(const SA& o) const;
		virtual size_t hash() const;
		virtual void print() const;
};
#endif
