#include "Cranknicolson.h"

algorithms::CrankNicolson::CrankNicolson() {

}

void algorithms::CrankNicolson::calcNextStep(QVector<double> const &last, QVector<double>& next, QVector< QVector<double>* > const &heatSources) {
    QVector<double> heatSources1 = *(heatSources[1]);
    QVector<double> heatSources2 = *(heatSources[2]);
    QVector<double> rhs = rhsMatrix * last + this->deltaT/2 * (heatSources2 + heatSources1);
    this->activeIterativeSolver->solve(next,this->itMatrix,rhs);
}

void algorithms::CrankNicolson::getNeedetHeatSources(QVector<double> &neededTimeSteps, bool &reusable) {
    reusable = true;
    neededTimeSteps.resize(2);
    neededTimeSteps[1]=1;
    neededTimeSteps[2]=0;
}

void algorithms::CrankNicolson::setUp(QVector<double> const &thermalConductivities) {
    CRS A1, tmp; A1.A1(IntMethod::n); A1 = tmp.diag(thermalConductivities)*A1;
    CRS eye; eye.eye(IntMethod::n);
    A1 = deltaT/(2*deltaX*deltaX) * A1;
    itMatrix = eye - A1;
    rhsMatrix = eye + A1;
}
