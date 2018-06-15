#include "CreateSolver.h"

#include "source/LinearSystem.h"
#include "Solver.h"
#include "SolverGPU.h"

ISolver* CreateSolver(const std::shared_ptr<LinearSystem> system, bool useGPU)
{
	if(useGPU)
		return new SolverGPU(system);
	else
		return new Solver(system);
};
