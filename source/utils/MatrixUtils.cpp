#include "MatrixUtils.h"

#include <vector>

#include "source/Matrix.h"

using namespace std;

vector<double> operator*(double alpha, const vector<double>& v)
{
	vector<double> temp;
	temp.resize(v.size());
	for(size_t i = 0; i < v.size(); i++)
		temp[i] = v[i] * alpha;
	return temp;
}

vector<double> operator*(const Matrix& A, const vector<double>& v)
{
	vector<double> temp;
	temp.resize(v.size());

	for(size_t i = 0; i < v.size(); i++)
		temp[i] = A.di[i] * v[i];

	for(size_t i = 0; i < v.size(); i++)
		for(int j = A.ig[i]; j < A.ig[i + 1]; ++j)
		{
			temp[i] += A.ggl[j] * v[A.jg[j]];
			temp[A.jg[j]] += A.ggl[j] * v[i];
		}

	return temp;
}

double FindScalar(const vector<double>& vec1, const vector<double>& vec2)
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
