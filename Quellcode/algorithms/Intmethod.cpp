#include "Intmethod.h"

algorithms::IntMethod::IntMethod() {

}

algorithms::IntMethod::~IntMethod() {

}

void algorithms::IntMethod::selectIterativeSolver(IterativeSolver *iterativeSolver) {
    activeIterativeSolver = iterativeSolver;
}

void algorithms::IntMethod::setUp(int const n, int const m, double const T, QVector<double> const &thermalConductivities) {
    this->n = n;
    this->deltaX = (1./(double)(n-1));
    this->deltaT = (T/(double)m);
    setUpItMatrix();
}

QVector<double> algorithms::operator*(double const &scalar, QVector<double> const &rhs) {
    QVector<double> mult = rhs;
    for(int i=0; i<rhs.size(); i++) mult[i] *= scalar;
    return mult;
}
