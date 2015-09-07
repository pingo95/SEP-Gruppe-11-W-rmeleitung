#include "Impeuler.h"

void algorithms::ImpEuler::setUpItMatrix() {
    CRS A1; A1.A1(IntMethod::n);
    CRS eye; eye.eye(IntMethod::n);
    itMatrix = deltaT/(deltaX*deltaX) * itMatrix;
    itMatrix = eye - itMatrix;
}

