#include "Iterativesolver.h"

algorithms::IterativeSolver::IterativeSolver() {

}

algorithms::IterativeSolver::~IterativeSolver() {

}

double algorithms::norm2(QVector<double> const &vec) {
    double sum=0;
    for(int i=0; i<vec.size(); i++) {
        sum += vec[i]*vec[i];
    }
    sum = sqrt(sum);
    return sum;
}
