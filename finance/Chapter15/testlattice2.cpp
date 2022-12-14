// testlattice.cpp
//
// Testing the Latice class (basis for binomial and trinomial
// trees)
//
// In this version we have one class for both binomial and trinomial
// trees.
//
//	2005-1-5 DD Using namespaces
//
// (C) Datasim Education BV 2003-2005
//

#include "lattice.cpp"
//#include "arraymechanisms.cpp"
#include "testlatticenamespace.cpp"
#include <math.h>

#include <iostream>
using namespace std;

template <class V, class I> Vector<V,I> CallPayOff(const V& K, const Vector<V,I>& StockArr)
{

		V tmp;

		Vector<V,I> result (StockArr.Size(), StockArr.MinIndex());

		for (I i = StockArr.MinIndex(); i <= StockArr.MaxIndex(); i++)
		{
			result[i] = 0.0;
			tmp = StockArr[i] - K;
			if (tmp > 0.0)
				result[i] = tmp;
		}

		return result;
}

template <class V, class I> Vector<V,I> PutPayOff(const V& K, const Vector<V,I>& StockArr)
{

		V tmp;

		Vector<V,I> result (StockArr.Size(), StockArr.MinIndex());

		for (I i = StockArr.MinIndex(); i <= StockArr.MaxIndex(); i++)
		{
			result[i] = 0.0;
			tmp = K - StockArr[i];
			if (tmp > 0.0)
				result[i] = tmp;
		}

		return result;
}


template <class V, class I, int NumberNodes> void print(const Lattice<V, I, NumberNodes>& source)
{
	
	for (I j = source.MinIndex(); j <= source.MaxIndex(); j++)
	{

		cout << "\nBranch Number " << j << ": [";
		for (I i = source[j].MinIndex(); i <= source[j].MaxIndex(); i++)
		{
			cout << source[j][i] << ", ";
	
		}

		cout << "]";
			
	}

}



int main()
{
	cout << "Forward sweep, Wilmott, Student Edition p. 193\n";

	int depth;
	cout << "How many subdivisions? ";
	cin >> depth;

	const n = 2;
	Lattice<double, int, n> lattice1(depth);

	print(lattice1);

	using namespace StandardLattice;

	double rootValue = 12.0;		 // Stock price now
	double D0 = 0.04;		// Dividend yield
	double vol = 0.3;		// Volatility
	double r = 0.06;		// Interest
	T = 1.0;				// Time to expiry

	k = T / double (depth);
	double e = ::exp((r-D0)*k);
	double sr = ::sqrt(exp(vol*vol*k) - 1.0);

	double up = e * (1.0 + sr);
	double down = e * (1.0 - sr);

	Lattice<double, int, 2> newLattice = createLattice(depth, rootValue, up, down);
//	print (newLattice);

	cout << "\nSize of base pyramid vector: " 
					<< newLattice.BasePyramidVector().Size() << endl;

	// Now work back from the payoff function

	// First make the discrete payoff
	double K = 10.0;
	Vector<double, int> RHS = CallPayOff(K, newLattice[newLattice.MaxIndex()]);
	Vector<double, int> RHS2 = PutPayOff(K, lattice1[lattice1.MaxIndex()]);

	double  p = 0.5;
	double discounting = ::exp(- r*k);
	
	double optionPrice = traverse(newLattice, RHS, p, discounting);
	double optionPrice2 = traverse(newLattice, RHS2, p, discounting);

	cout << "\nOption price C " << optionPrice << endl;
	cout << "\nOption price P " << optionPrice2 << endl;

	return 0;
}
