#ifndef __ACTION__H__
#define __ACTION__H__

#include <cstddef>
#include <vector>

class Action{
	protected:
		using type = std::size_t;
		type _ID; //identifier
	public:
		virtual type ID() const;
		Action(type a);
		bool operator==(const Action& a) const;
		virtual void print() const;
};

namespace std{
	template<>
	struct hash<Action>
	{
		size_t operator()(const Action& x) const{
			return x.ID();
		}
	};
}


#endif
