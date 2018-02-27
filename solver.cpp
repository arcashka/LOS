#include "solver.h"
#include "iostream"

#include "matrix.h"

vector<double> operator*(double alpha, const vector<double>& v)
{
	vector<double> temp;
	temp.resize(v.size());
	for(size_t i = 0; i < v.size(); i++)
		temp[i] = v[i] * alpha;
	return temp;
}

vector<double> operator*(shared_ptr<Matrix> A, const vector<double>& v)
{
	vector<double> temp;
	temp.resize(v.size());

	for(size_t i = 0; i < v.size(); i++)
		temp[i] = A->di[i] * v[i];

	for(size_t i = 0; i < v.size(); i++)
		for(int j = A->ig[i]; j < A->ig[i + 1]; ++j)
		{
			temp[i] += A->ggl[j] * v[A->jg[j]];
			temp[A->jg[j]] += A->ggl[j] * v[i];
		}

	return temp;
}

double Solver::FindScalar(const vector<double>& vec1, const vector<double>& vec2)
{
	double scalar = 0;
	for(size_t i = 0; i < vec1.size(); i++)
		scalar += vec1[i] * vec2[i];

	return scalar;
}

vector<double> operator+(const vector<double>& v1, const vector<double>& v2)
{
	vector<double> temp;
	temp.resize(v1.size());
	for(size_t i = 0; i < v1.size(); i++)
		temp[i] = v1[i] + v2[i];
	return temp;
}

vector<double> operator-(const vector<double>& v1, const vector<double>& v2)
{
	vector<double> temp;
	temp.resize(v1.size());
	for(size_t i = 0; i < v1.size(); i++)
		temp[i] = v1[i] - v2[i];
	return temp;
}

template<typename T>
ostream& operator<<(ostream& out, const vector<T>& vec)
{
	for(size_t i = 0; i < vec.size(); ++i)
		out << vec[i] << " ";
	return out;
}

Solver::Solver(shared_ptr<Matrix> a, vector<double>& b) :
	a(a), b(b)
{
}

vector<double> Solver::Solve(vector<double>& x0, double eps, int maxItt)
{
	vector<double> r = b - a * x0;
	vector<double> z = r;
	vector<double> x = x0;

	double alpha;
	double beta;
	double bbScalar = FindScalar(b, b);

	cout << "0" << "\t" << "-> " << x << "\n";


	for(int k = 0; k < maxItt; k++)
	{
		double rrScalar = FindScalar(r, r);
		alpha = rrScalar / FindScalar(a * z, z);
		x = x + (alpha * z);
		r = r - (alpha * (a * z));
		beta = FindScalar(r, r) / rrScalar;
		z = r + (beta * z);

		double diff = rrScalar / bbScalar;
		cout << k + 1 << "\t" << "diff = " << diff << "\t" << "-> " << x << "\n";

		if(diff < eps)
			break;
	}
	return x;
}
