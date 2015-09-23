#include "Intmethod.h"

template <class T>
algorithms::IntMethod<T>::IntMethod() {

}

template <class T>
algorithms::IntMethod<T>::~IntMethod() {

}

template <class T>
algorithms::Solver<T>* const & algorithms::IntMethod<T>::getSolver() const {
    return this->activeSolver;
}

template <class T>
void algorithms::IntMethod<T>::selectSolver(Solver<T>* solver) {
    this->activeSolver = solver;
}

template <class T>
void algorithms::IntMethod<T>::setUp(int const n, int const m, T const Time, QVector<T> const & thermalDiffusivities) {
    this->n = n;
    this->deltaX = (1./(T)(n-1));
    this->deltaT = (Time/(T)m);
    setUpSpecific(thermalDiffusivities);
    this->activeSolver->decompose(this->itMatrix);
}
