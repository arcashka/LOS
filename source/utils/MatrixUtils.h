#pragma once

#include <vector>

struct Matrix;

std::vector<float> operator*(float alpha, const std::vector<float>& v);

std::vector<float> operator*(const Matrix& A, const std::vector<float>& v);

float FindScalar(const std::vector<float>& vec1, const std::vector<float>& vec2);

std::vector<float> operator+(const std::vector<float>& v1, const std::vector<float>& v2);

std::vector<float> operator-(const std::vector<float>& v1, const std::vector<float>& v2);
