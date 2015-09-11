#include "Jacobi.h"

algorithms::Jacobi::Jacobi() {

}

void algorithms::Jacobi::solve(QVector<double> &result, CRS const &matrix, QVector<double> const &rhs) {
    double rel=1;
    QVector<double> res(result.size()), old = result;
    while(rel-eps>0 && itCount < maxIt) {
        for(int i=0; i<result.size(); ++i) {
            assert(matrix.getValue(i,i)!=0);
            double sum=0;
            int lb = matrix.getRowsNumElem(i); lb = matrix.getIndex(lb);
            int ub = matrix.getRowsNumElem(i+1); ub = matrix.getIndex(ub-1);
            int ub1 = i < ub ? i : ub;
            if(ub1==i) {
                for(int j=lb; j<ub1; ++j) {
                    sum += matrix.getValue(i,j) * old[j];
                }
                for(int j=i+1; j<=ub; ++j) {
                    sum += matrix.getValue(i,j) * old[j];
                }
            }
            else {
                for(int j=lb; j<=ub; ++j) {
                    sum += matrix.getValue(i,j) * old[j];
                }
            }
            result[i] = 1/matrix.getValue(i,i) * (rhs[i]-sum);
        }
        old = result;
        res = matrix*result;
        res = algorithms::addQVectors(res,(-1.) * rhs);
        rel = norm2(res)/norm2(rhs);
        ++itCount;
    }

}
