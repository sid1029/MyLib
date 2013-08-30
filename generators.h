#ifndef GENERATORS_H
#define GENERATORS_H

#include "CoreTypes.h"

/*
* Used to generate consecutive numbers starting at n.
*/
template <typename T>
class Sequence
{
public:
	Sequence(T n) : x(n - 1) {}
	inline T operator() () { return ++x; }
private:
	T x;
};

/*
* Used to generate random numbers between a & b.
*/
template <typename T>
class RandomNos
{
public:
	RandomNos(T a, T b) : _a(a), _b(b) {}
	inline T operator() () { return _a + ( rand()%(_b - _a) ); }
private:
	T _a, _b;
};

/*
* Used to print an std::vector.
*/
template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& obj)
{
	size_t s = obj.size();
	for(size_t i = 0; i < s; ++i)
		os<<setw(2)<<obj[i]<<" ";
	return os;
}

#endif // GENERATORS_H