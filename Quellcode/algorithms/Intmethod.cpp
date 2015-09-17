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

    equi.resize(itMatrix.getSize());
    for(int i=0; i<equi.size(); ++i)
        equi[i]=1;

    activeSolver->equilibrate(itMatrix,equi);
    activeSolver->decompose(itMatrix);
}
