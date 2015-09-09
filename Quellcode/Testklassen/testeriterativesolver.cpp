#include "testeriterativesolver.h"

algorithms::TesterIterativeSolver::TesterIterativeSolver() {
    text = new QLabel();
}

QLabel* algorithms::TesterIterativeSolver::print() {
    return text;
}
