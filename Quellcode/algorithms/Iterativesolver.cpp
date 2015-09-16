#include "Iterativesolver.h"

algorithms::IterativeSolver::IterativeSolver() {

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

void algorithms::IterativeSolver::setEps(double const eps) {
    this->eps = eps;
}

void algorithms::IterativeSolver::setMaxIt(const int maxIt) {
    this->maxIt = maxIt;
}
