#include "Cranknicolson.h"

algorithms::CrankNicolson::CrankNicolson() {

}

void algorithms::CrankNicolson::calcNextStep(QVector<double> const &last, QVector<double> &next, QVector<double> const &heatSources) {

}

void algorithms::CrankNicolson::setUpItMatrix() {
    CRS A1; A1.A1(IntMethod::n);
    CRS eye; eye.eye(IntMethod::n);
    itMatrix = deltaT/(deltaX*deltaX) * itMatrix * A1;
    itMatrix = eye - itMatrix;
}

