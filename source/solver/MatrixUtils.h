#pragma once

#include <vector>
#include <memory>

struct Matrix;

std::vector<double> operator*(double alpha, const std::vector<double>& v);

std::vector<double> operator*(std::shared_ptr<Matrix> A, const std::vector<double>& v);

double FindScalar(const std::vector<double>& vec1, const std::vector<double>& vec2);

std::vector<double> operator+(const std::vector<double>& v1, const std::vector<double>& v2);

std::vector<double> operator-(const std::vector<double>& v1, const std::vector<double>& v2);
