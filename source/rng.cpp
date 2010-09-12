#include "rng.h"
#include <ctime>
#include <cmath>
#include <cstdlib>

namespace
{
using namespace std;

const double _e = 0.00000001;
}


RNG::RNG(const long s) { srandom(s); }
RNG::RNG() { srandom(time(NULL)); }

double RNG::max() { return double(RAND_MAX); }

long RNG::draw() { return random(); }
double RNG::fdraw() { return draw()/max(); }
long RNG::operator()() { return draw(); }


M_RNG::M_RNG(const long nn) : n(nn) { }

void M_RNG::set_lim(const long lim) { n = lim; }

long M_RNG::operator()()
{
	long r = (long)(n*fdraw());
	return (r==n) ? n-1 : r;
}

int M_RNG::index(const int m)
{
	long r = (long)(m*fdraw());
	return (r==m) ? m-1 : r;
}

int M_RNG::pm(const int a)
{
	if(a == 0) return n - 2 * (*this)();
	else return a - 2 * index(a);
}


bool B_RNG::one_in(const int x)
{
	if(x <= 0)
		return false;
	if(x == 1)
		return true;
	return !(draw() % x); // 1 in x chance
}


N_RNG::N_RNG(const long m, const long d) : mean(m), dist(d) { }

void N_RNG::set_mean(const long m) { mean = m; }
void N_RNG::set_dist(const long d) { dist = d; }
void N_RNG::setup(const long m, const long d) { set_mean(m); set_dist(d); }
long N_RNG::operator()() { double d = fdraw(); if(d < _e) d = _e; return (long)(sqrt(-2*log(d))*(double)dist + (double)mean); }

int N_RNG::index(const int m)
{
	set_mean(0);
	set_dist(m > 0 ? long(ceil(float(m)/3)) : 1); // explicit cast to long to avoid warning
	int ind;
	do ind = (*this)();
	while(ind >= m);
	return ind;
}

int N_RNG::pm(const int a)
{
	set_mean(0);
	set_dist(a > 0 ? long(ceil(float(a)/3)) : 1); // explicit cast to long to avoid warning
	int retval;
	do retval = (*this)();
	while(retval >= a);
	if(draw() <= (long)(0.5*max()))
		retval *= -1;
	return retval;
}
