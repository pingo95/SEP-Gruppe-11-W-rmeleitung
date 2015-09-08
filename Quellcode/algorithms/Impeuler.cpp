#include "Impeuler.h"

algorithms::ImpEuler::ImpEuler() {

}

void algorithms::ImpEuler::setUpItMatrix() {
    CRS A1; A1.A1(IntMethod::n);
    CRS eye; eye.eye(IntMethod::n);
    itMatrix = deltaT/(deltaX*deltaX) * itMatrix * A1;
    itMatrix = eye - itMatrix;
}

