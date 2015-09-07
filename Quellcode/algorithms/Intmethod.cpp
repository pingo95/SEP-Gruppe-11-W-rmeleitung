#include "Intmethod.h"

algorithms::IntMethod::IntMethod() {

}

void algorithms::IntMethod::calcNextStep(const QVector<double> &last, QVector<double> &next, const QVector<double> &heatSources) {
    QVector<double> rhs = last + this->deltaT * heatSources;
    this->activeIterativeSolver->solve(next,this->itMatrix,rhs);
}

void algorithms::IntMethod::selectIterativeSolver(IterativeSolver *iterativeSolver) {

}


void algorithms::IntMethod::setThermalConductivities(QVector<double> const &thermalConductivities) {

}

void algorithms::IntMethod::setUp(int const n, int const m, double const T, QVector<double> const &thermalConductivities) {
    this->n = n;
    this->deltaX = (1./(double)(n-1));
    this->deltaT = (T/(double)m);
    setThermalConductivities(thermalConductivities);
    setUpItMatrix();
}

QVector<double> algorithms::operator *(double const &scalar, QVector<double> const &rhs) {
    QVector<double> tmp = rhs;
    for(int i=0; i<rhs.size(); i++) tmp[i] *= scalar;
}
