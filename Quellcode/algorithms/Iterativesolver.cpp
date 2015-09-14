#include "Iterativesolver.h"

algorithms::IterativeSolver::IterativeSolver() : eps(1e-10), itCount(0), maxIt(1000){

}

algorithms::IterativeSolver::~IterativeSolver() {

}

double algorithms::IterativeSolver::getEps() const {
    return eps;
}

int algorithms::IterativeSolver::getItCount() const {
    return itCount;
}

int algorithms::IterativeSolver::getMaxIt() const {
    return maxIt;
}

void algorithms::IterativeSolver::setEps(double eps) {
    this->eps = eps;
}

void algorithms::IterativeSolver::setMaxIt(int maxIt) {
    this->maxIt = maxIt;
}

double algorithms::norm2(QVector<double> const &vec) {
    double sum=0;
    for(int i=0; i<vec.size(); ++i) {
        sum += vec[i]*vec[i];
    }
    sum = sqrt(sum);
    return sum;
}
