#include "Gaussseidel.h"

algorithms::GaussSeidel::GaussSeidel() {

}

void algorithms::GaussSeidel::solve(QVector<double> &result, CRS const &matrix, QVector<double> const &rhs) {
    double rel=1, sum;
    itCount = 0;
    QVector<double> res(result.size());
    while(rel-eps>0 && itCount < maxIt) {
        for(int i=0; i<result.size(); ++i) {
            assert(matrix.getValue(i,i)!=0);
            sum=0;
        sum=matrix.multRowQVector(i,result);
        result[i] += 1./matrix.getValue(i,i) * (rhs[i]-sum);
        }
        rel = norm2(algorithms::addQVectors(matrix*result,(-1.) * rhs))/norm2(rhs);
        ++itCount;
    }
}
