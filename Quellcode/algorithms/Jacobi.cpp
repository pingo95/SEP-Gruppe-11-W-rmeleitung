#include "Jacobi.h"

algorithms::Jacobi::Jacobi() {

}

void algorithms::Jacobi::solve(QVector<double> & result, CRS const & matrix, QVector<double> const & rhs) {
    double rel=1, sum;
    itCount = 0;

    QVector<double> old = result;
    while(rel-eps>0 && itCount < maxIt) {
        for(int i=0; i<result.size(); ++i) {
            assert(matrix.getValue(i,i)!=0);
            sum=matrix.multRowQVector(i,old);
            result[i] += 1./matrix.getValue(i,i)*(rhs[i]-sum);
        }
        old = result;
//        rel = norm2(addQVectors(matrix*result,(-1.) * rhs))/norm2(rhs);
        rel = normInf(addQVectors(matrix*result,(-1.) * rhs))/normInf(rhs);
        ++itCount;
    }
}
