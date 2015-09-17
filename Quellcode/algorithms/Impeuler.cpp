#include "Impeuler.h"

algorithms::ImpEuler::ImpEuler() {

}

void algorithms::ImpEuler::calcNextStep(QVector<double> const &last, QVector<double>& next, QVector< QVector<double>* > const &heatSources) {
    QVector<double> rhs = algorithms::addQVectors(last,deltaT * *(heatSources[0]));
    activeSolver->solve(next,itMatrix,rhs);
}

void algorithms::ImpEuler::getNeedetHeatSources(QVector<double> &neededTimeSteps, bool &reusable) const {
    reusable = true;
    neededTimeSteps.resize(1);
    neededTimeSteps[0]=1;
}

void algorithms::ImpEuler::setUpSpecific(QVector<double> const &thermalConductivities) {
    algorithms::CRS A1, diag;
    A1.A1(n);
    diag.diag(thermalConductivities);
    diag = diag.multCRSCRS(A1);
    A1.eye(n);
    itMatrix = deltaT/(deltaX*deltaX) * diag;
    itMatrix = A1 - itMatrix;
}
