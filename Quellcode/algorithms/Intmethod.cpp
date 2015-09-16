#include "Intmethod.h"

algorithms::IntMethod::IntMethod() {

}

algorithms::IntMethod::~IntMethod() {

}

algorithms::Solver * const & algorithms::IntMethod::getSolver() const
{
    return activeSolver;
}

void algorithms::IntMethod::selectSolver(Solver* solver) {
    activeSolver = solver;
}

void algorithms::IntMethod::setUp(int const n, int const m, double const T, QVector<double> const &thermalConductivities) {
    this->n = n;
    deltaX = (1./(double)(n-1));
    deltaT = (T/(double)m);
    setUpSpecific(thermalConductivities);
    activeSolver->decompose(itMatrix);
}
