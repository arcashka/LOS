#pragma once

#include <memory>

struct LinearSystem;
class  ISolver;

ISolver* CreateSolver(const std::shared_ptr<LinearSystem> system, bool useGPU);
