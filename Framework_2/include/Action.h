#ifndef __ACTION_H__
#define __ACTION_H__
#include <cstddef>
#include <memory>
class Action;
using a_ptr = std::shared_ptr<Action>;
class Action{
	protected:
		size_t _ID;
		size_t _hash;
	public:
		Action(size_t i, size_t h);
		virtual ~Action();
		virtual bool operator==(const Action&) const;
		virtual size_t hash() const;
		size_t ID() const;
		virtual void print() const;
};
namespace std{
	template<>
	struct hash<Action>{
		size_t operator()(const Action& x) const{
			return x.hash();
		}
	};
}

#endif
