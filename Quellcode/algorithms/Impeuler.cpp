#include "Impeuler.h"

algorithms::ImpEuler::ImpEuler() {

}

void algorithms::ImpEuler::calcNextStep(QVector<double> const &last, QVector<double>& next, QVector< QVector<double>* > const &heatSources) {
    QVector<double> heatSources1 = *(heatSources[1]);
    QVector<double> rhs = last + this->deltaT * heatSources1;
    this->activeIterativeSolver->solve(next,this->itMatrix,rhs);
}

void algorithms::ImpEuler::getNeedetHeatSources(QVector<double> &neededTimeSteps, bool &reusable) {
    reusable = true;
    neededTimeSteps.resize(1);
    neededTimeSteps[1]=1;
}

void algorithms::ImpEuler::setUp(QVector<double> const &thermalConductivities) {
    CRS A1, tmp; A1.A1(IntMethod::n); A1 = tmp.diag(thermalConductivities)*A1;
    CRS eye; eye.eye(IntMethod::n);
    itMatrix = deltaT/(deltaX*deltaX) * A1;
    itMatrix = eye - itMatrix;
}

