#include "Intmethod.h"

algorithms::IntMethod::IntMethod() {

}

algorithms::IntMethod::~IntMethod() {

}

void algorithms::IntMethod::selectIterativeSolver(IterativeSolver *iterativeSolver) {
    activeIterativeSolver = iterativeSolver;
}

QVector<double> algorithms::operator*(double const &scalar, QVector<double> const &rhs) {
    QVector<double> mult = rhs;
    for(int i=0; i<rhs.size(); i++) mult[i] *= scalar;
    return mult;
}
