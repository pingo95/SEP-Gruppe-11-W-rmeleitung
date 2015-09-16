#include "Cranknicolson.h"

algorithms::CrankNicolson::CrankNicolson() {

}

void algorithms::CrankNicolson::calcNextStep(QVector<double> const &last, QVector<double>& next, QVector< QVector<double>* > const &heatSources) const {
    QVector<double> rhs = algorithms::addQVectors(rhsMatrix * last,this->deltaT/2 * (algorithms::addQVectors(*(heatSources[1]),*(heatSources[0]))));
    this->activeSolver->solve(next,this->itMatrix,rhs);
}

void algorithms::CrankNicolson::getNeedetHeatSources(QVector<double> &neededTimeSteps, bool &reusable) const {
    reusable = true;
    neededTimeSteps.resize(2);
    neededTimeSteps[0]=1;
    neededTimeSteps[1]=0;
}

void algorithms::CrankNicolson::setUpSpecific(QVector<double> const &thermalConductivities) {
    algorithms::CRS A1, diag;
    A1.A1(n);
    diag.diag(thermalConductivities);
    diag = diag.multCRSCRS(A1);
    A1.eye(n);
    diag = deltaT/(2*deltaX*deltaX) * diag;
    itMatrix = A1 - diag;
    rhsMatrix = A1 + diag;
}
