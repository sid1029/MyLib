#ifndef GENERATORS_H
#define GENERATORS_H


#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "dynamic.h"
#include "CoreTypes.h"

using namespace std;

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
	RandomNos(T a, T b) : _a(a), _diff(b-a) {}
	inline T operator() () { return _a + ( rand() % _diff ); }
private:
	T _a, _diff;
};

/*
* Used to generate random numbers between a & b.
*/
class RandomGpas
{
public:
	RandomGpas(int a, int b) : _a(a), _diff(b - a) {}
	inline Dynamic::SatGpa operator() (const Dynamic::SatGpa& elem) { return Dynamic::SatGpa(_a + ( rand() % _diff ), _a + ( rand() % _diff )); }
private:
	int _a, _diff;
};

/*
* Used to generate increasing numbers starting at n and repeated randomly.
*/
template <typename T>
class RandomSorted
{
public:
	RandomSorted(T n) : _n(n), _curr(0) {}
	inline T operator() ()
	{
		if (_curr >= _n) return _n;
		if (rand() % 100 < 40) return _n;
		return _n += ( rand() % 17 );
	}
private:
	T _n, _curr;
};

/*
* Used to print an std::vector.
*/
template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& obj)
{
	size_t s = obj.size();
	os<<"[";
	for(size_t i = 0; i < s; ++i)
		os<<setw(2)<<obj[i]<<" ";
	os<<"]";
	return os;
}

#endif // GENERATORS_H