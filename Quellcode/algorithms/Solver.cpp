#include "Solver.h"

algorithms::Solver::Solver() : eps(1e-10), itCount(0), maxIt(1000) {

}

algorithms::Solver::~Solver() {

}

void algorithms::Solver::decompose(CRS const & matrix) {

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

void algorithms::Solver::setEps(double const eps) {

}

void algorithms::Solver::setMaxIt(int const maxIt) {

}

double algorithms::norm2(QVector<double> const & vec) {
    double sum=0;
    for(int i=0; i<vec.size(); ++i) {
        sum += vec[i]*vec[i];
    }
    sum = sqrt(sum);
    return sum;
}

double algorithms::normInf(QVector<double> const & vec) {
    double max=0;
    for(int i=0; i<vec.size(); ++i) {
        if(max<fabs(vec[i]))
            max = fabs(vec[i]);
    }
    return max;
}
