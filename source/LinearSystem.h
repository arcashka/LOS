#pragma once

#include "Matrix.h"

struct LinearSystem
{
	Matrix matrix;
	std::vector<double> b;
};
