#include "Iterativesolver.h"

algorithms::IterativeSolver::IterativeSolver() : eps(1e-5), itCount(0) {

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
