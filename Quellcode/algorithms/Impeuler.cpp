#include "Impeuler.h"

algorithms::ImpEuler::ImpEuler() {

}

void algorithms::ImpEuler::calcNextStep(const QVector<double> &last, QVector<double> &next, const QVector<double> &heatSources) {
    QVector<double> rhs = last + this->deltaT * heatSources;
    this->activeIterativeSolver->solve(next,this->itMatrix,rhs);
}

void algorithms::ImpEuler::setUp(const int n, const int m, const double T, const QVector<double> &thermalConductivities) {
    this->n = n;
    this->deltaX = (1./(double)(n-1));
    this->deltaT = (T/(double)m);
    setUpItMatrix();
}

void algorithms::ImpEuler::setUpItMatrix() {
    CRS A1; A1.A1(IntMethod::n);
    CRS eye; eye.eye(IntMethod::n);
    itMatrix = deltaT/(deltaX*deltaX) * itMatrix * A1;
    itMatrix = eye - itMatrix;
}

