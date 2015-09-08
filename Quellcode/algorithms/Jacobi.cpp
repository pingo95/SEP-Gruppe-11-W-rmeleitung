#include "Jacobi.h"

algorithms::Jacobi::Jacobi() : eps(1e-5), itCount(0) {

}

void algorithms::Jacobi::solve(QVector<double> &result, CRS const &matrix, QVector<double> const &rhs) {
    double rel=1;
    while(rel-eps>0) {
        for(int i=0; i<result.size(); ++i) {
            double sum=0;
            int lb = matrix.getRowsMinCol(i);
            int ub = matrix.getRowsMinCol(i+1);
            int ub1 = i < ub ? i : ub;
            if(ub1==i) {
                for(int j=lb; j<ub1; ++j) {
                    sum += matrix.getValue(i,j) * result[j];
                }
                for(int j=i+1; j<ub; ++j) {
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
        rel = norm2(matrix*result + ((-1.)*rhs))/norm2(rhs);
        ++itCount;
    }

}
