#include "LU.h"

algorithms::LU::LU() {

}

void algorithms::LU::decompose(CRS const & matrix) {
    L.resize(matrix.getSize());
    for(int i=0; i<L.size(); ++i) {
        L[i].resize(L.size());
        L[i][i] = 1;
    }

    QVector<QVector<double> > full;
    matrix.full(full);
    U = full;

    for(int i=0; i<L.size()-1; ++i) {
        assert(U[i][i]!=0);
        for(int k=i+1; k<L.size(); ++k) {
            L[k][i] = U[k][i] / U[i][i];
            for(int j=i; j<L.size(); ++j)
                U[k][j] -= L[k][i] * U[i][j];
        }
    }

}

void algorithms::LU::solve(QVector<double> & result, CRS const & matrix, QVector<double> const & rhs) {
    QVector<double> y;
    y.resize(result.size());
    double sum;
    for(int i=0; i<y.size(); ++i) {
        sum=0;
        for(int j=0; j<i; ++j) {
            sum += L[i][j]*y[j];
        }
        double tmp = rhs[i]-sum;
        y[i] = 1/L[i][i]*(tmp);
    }
    for(int i=y.size()-1; i>=0; --i) {
        sum=0;
        for(int j=i+1; j<y.size(); ++j) {
            sum += U[i][j]*result[j];
        }
        result[i] = 1/U[i][i]*(y[i]-sum);
    }
}
