#include "LU.h"

algorithms::LU::LU() {

}

void algorithms::LU::decompose(CRS const & matrix) {
    QVector<QVector<double> > full;
    matrix.full(full);
    L.resize(matrix.getSize());
    for(int i=0; i<L.size(); ++i) {
        L[i].resize(L.size());
        for(int j=0; j<i; ++j) L[i][j] = 0;
        L[i][i] = 1;
        for(int j=i+1; j<L.size(); ++j) L[i][j] = 0;
    }
    U = full;

    for(int i=0; i<L.size()-1; ++i) {
        assert(U[i][i]!=0);
        for(int k=i+1; k<L.size(); ++k) {
            L[k][i] = U[k][i] / U[i][i];
            for(int j=i; j<L.size(); ++j)
                U[k][j] = U[k][j] - L[k][i] * U[i][j];
        }
    }

}

void algorithms::LU::solve(QVector<double> & result, CRS const & matrix, QVector<double> const & rhs) {
    QVector<double> tmp;
    tmp.resize(result.size());
    double sum;
    for(int i=0; i<tmp.size(); ++i) {
        sum=0;
        for(int j=0; j<i; ++j) {
            sum += tmp[i];
        }
        tmp[i] = rhs[i]+sum;
    }
    for(int i=tmp.size()-1; i>=0; --i) {
        sum=0;
        for(int j=tmp.size()-1; j>=i; --j) {
            sum += result[i];
        }
        tmp[i] = rhs[i]+sum;
    }

}
