#include "Solver.h"

algorithms::Solver::Solver() : eps(1e-10), itCount(0), maxIt(1000){

}

algorithms::Solver::~Solver() {

}

void algorithms::Solver::decompose(CRS & matrix) {

}

void algorithms::Solver::equilibrate(CRS & matrix, QVector<double> & rhs) {
    double sum;
    QVector<double> ones(matrix.getSize()), diag(matrix.getSize());
    CRS equi;
    for(int i=0; i<matrix.getSize(); ++i)
        ones[i]=1;

    for(int i=0; i<matrix.getSize(); ++i) {
        sum = matrix.multRowQVectorAbs(i,ones);
        diag[i] = 1/sum;
        rhs[i] *= 1/sum;
    }
    equi.diag(diag);
    matrix = equi.multCRSCRS(matrix);
}

double algorithms::Solver::getEps() const {
    return 0;
}

int algorithms::Solver::getItCount() const {
    return 1;
}

int algorithms::Solver::getMaxIt() const {
    return 1;
}

void algorithms::Solver::setEps(double eps) {

}

void algorithms::Solver::setMaxIt(int maxIt) {

}

double algorithms::norm2(QVector<double> const &vec) {
    double sum=0;
    for(int i=0; i<vec.size(); ++i) {
        sum += vec[i]*vec[i];
    }
    sum = sqrt(sum);
    return sum;
}
