#include "Gaussseidel.h"

algorithms::GaussSeidel::GaussSeidel() {

}

void algorithms::GaussSeidel::solve(QVector<double> &result, CRS const &matrix, QVector<double> const &rhs) {
    double rel=1;
    QVector<double> res(result.size());
    while(rel-eps>0) {
        for(int i=0; i<result.size(); ++i) {
            double sum=0;
            int lb = matrix.getRowsNumElem(i); lb = matrix.getIndex(lb);
            int ub = matrix.getRowsNumElem(i+1); ub = matrix.getIndex(ub-1);
            int ub1 = i < ub ? i : ub;
            if(ub1==i) {
                for(int j=lb; j<ub1; ++j) {
                    sum += matrix.getValue(i,j) * result[j];
                }
                for(int j=ub1+1; j<ub; ++j) {
                    sum += matrix.getValue(i,j) * result[j];
                }
            }
            else {
                for(int j=lb; j<ub; ++j) {
                    sum += matrix.getValue(i,j) * result[j];
                }
            }
            result[i] = 1/matrix.getValue(i,i) * (rhs[i]-sum);
        }
        res = matrix*result;
        res = algorithms::addQVectors(res,(-1.) * rhs);
        rel = norm2(res)/norm2(rhs);
        ++itCount;
    }
}
