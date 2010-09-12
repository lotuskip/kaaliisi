#ifndef RNG_H
#define RNG_H

// These are just a wrapper for standard random().


class RNG // gives an even distribution in the set {0,1,2,...,max}
{
public:
	RNG(const long s);
	RNG();

	long operator()();
	double fdraw(); // returns a floating point number between 0 and 1

protected:
	long draw();

	static double max();
};


class M_RNG : public RNG // gives an even distribution in the set {0,1,2,...,n-1}
{
public:
	M_RNG(const long nn = 100);

	void set_lim(const long lim);

	long operator()();
	int index(const int m); // NOTE: This does NOT change the limit n of this object! (neither does the next one)
	int pm(const int a = 0); // returns random -a...+a, if a = 0, assume a = n

private:
	long n;
};


class B_RNG : public RNG
{
public:
	bool one_in(const int x); // does a "chance of 1 in x" test and returns the result
};


class N_RNG : public RNG // gives a Gaussian distribution with defined mean and deviation
{
public:
	N_RNG(const long m = 50, const long d = 10);

	void set_mean(const long m);
	void set_dist(const long d);
	void setup(const long m, const long d);

	long operator()();
	int index(const int m);
	int pm(const int a); // returns random -a...+a

private:
	long mean, dist;
};


extern M_RNG m_draw;
extern B_RNG b_draw;
extern N_RNG n_draw;

#endif
