#include "Intmethod.h"

algorithms::IntMethod::IntMethod() {

}

algorithms::IntMethod::~IntMethod() {

}

double algorithms::IntMethod::getEps() {
    return this->activeIterativeSolver->getEps();
}

int algorithms::IntMethod::getItCount() {
    return this->activeIterativeSolver->getItCount();
}

int algorithms::IntMethod::getMaxIt() {
    return this->activeIterativeSolver->getMaxIt();
}

void algorithms::IntMethod::selectIterativeSolver(IterativeSolver *iterativeSolver) {
    activeIterativeSolver = iterativeSolver;
}

void algorithms::IntMethod::setEps(double eps) {
    this->activeIterativeSolver->setEps(eps);
}

void algorithms::IntMethod::setMaxIt(int maxIt) {
    this->activeIterativeSolver->setMaxIt(maxIt);
}

void algorithms::IntMethod::setUp(int const n, int const m, double const T, QVector<double> const &thermalConductivities) {
    this->n = n;
    this->deltaX = (1./(double)(n-1));
    this->deltaT = (T/(double)(m-1));
    this->setUpSpecific(thermalConductivities);
}
