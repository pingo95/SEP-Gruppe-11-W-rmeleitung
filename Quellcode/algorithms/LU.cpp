#include "LU.h"

algorithms::LU::LU() {

}

void algorithms::LU::decompose(CRS & matrix) {
    QVector<QVector<double> > full;
    equi.resize(matrix.getSize());
    //equilibrate(matrix,equi);
    matrix.full(full);
    L.resize(matrix.getSize());
    for(int i=0; i<L.size(); ++i) {
        L[i].resize(L.size());
        L[i][i] = 1;
    }
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

void algorithms::LU::solve(QVector<double> & result, CRS & matrix, QVector<double> & rhs) {
    equi.resize(rhs.size());
    for(int i=0; i<equi.size(); ++i)
        equi[i]=1;

    for(int i=0; i<rhs.size(); ++i)
        rhs[i] *= equi[i];

    QVector<double> tmp;
    tmp.resize(result.size());
    double sum;
    for(int i=0; i<tmp.size(); ++i) {
        sum=0;
        for(int j=0; j<i; ++j) {
            sum += L[i][j] * tmp[j];
        }
        tmp[i] = 1/L[i][i] * (rhs[i]-sum);
    }
    for(int i=tmp.size()-1; i>=0; --i) {
        sum=0;
        for(int j=i+1; j<tmp.size(); ++j) {
            sum += U[i][j] * result[j];
        }
        result[i] = 1/U[i][i] * (tmp[i]-sum);
    }

}
