#include "MatrixUtils.h"

#include <vector>

#include "source/Matrix.h"

using namespace std;

vector<float> operator*(float alpha, const vector<float>& v)
{
	vector<float> temp;
	temp.resize(v.size());
	for(size_t i = 0; i < v.size(); i++)
		temp[i] = v[i] * alpha;
	return temp;
}

vector<float> operator*(const Matrix& A, const vector<float>& v)
{
	vector<float> temp;
	temp.resize(v.size());

	for(size_t i = 0; i < v.size(); i++)
		temp[i] = A.di[i] * v[i];

	if (A.ig.empty())
		return temp;

	for(size_t i = 0; i < v.size(); i++)
		for(int j = A.ig[i]; j < A.ig[i + 1]; ++j)
		{
			temp[i] += A.ggl[j] * v[A.jg[j]];
			temp[A.jg[j]] += A.ggl[j] * v[i];
		}

	return temp;
}

float FindScalar(const vector<float>& vec1, const vector<float>& vec2)
{
	float scalar = 0;
	for(size_t i = 0; i < vec1.size(); i++)
		scalar += vec1[i] * vec2[i];

	return scalar;
}

vector<float> operator+(const vector<float>& v1, const vector<float>& v2)
{
	vector<float> temp;
	temp.resize(v1.size());
	for(size_t i = 0; i < v1.size(); i++)
		temp[i] = v1[i] + v2[i];
	return temp;
}

vector<float> operator-(const vector<float>& v1, const vector<float>& v2)
{
	vector<float> temp;
	temp.resize(v1.size());
	for(size_t i = 0; i < v1.size(); i++)
		temp[i] = v1[i] - v2[i];
	return temp;
}
